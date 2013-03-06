#include "navigation.h"
#include "recast/Detour/Include/DetourNavMeshBuilder.h"
#include "recast/Detour/Include/DetourNavMesh.h"
#include "scene/SceneManager.h"
NAVIGATION *NAVIGATION_init( const char *name )
{
	NAVIGATION *navigation = ( NAVIGATION * ) calloc( 1, sizeof( NAVIGATION ) );

	strcpy( navigation->name, name );

	navigation->tolerance.x = 1.0f;
	navigation->tolerance.y = 2.0f;
	navigation->tolerance.z = 1.0f;
	
	NAVIGATION_set_default_configuration( navigation );

	return navigation;
}


NAVIGATION *NAVIGATION_free( NAVIGATION *navigation )
{
	if( navigation->dtnavmesh ) dtFreeNavMesh( navigation->dtnavmesh );	
	free( navigation );
	
	return NULL;
}


void NAVIGATION_set_default_configuration( NAVIGATION *navigation )
{
	navigation->navigationconfiguration.cell_size				= 0.3f;
	navigation->navigationconfiguration.cell_height				= 0.2f;
	navigation->navigationconfiguration.agent_height			= 2.0f;
	navigation->navigationconfiguration.agent_radius			= 0.4f;
	navigation->navigationconfiguration.agent_max_climb			= 0.9f;
	navigation->navigationconfiguration.agent_max_slope			= 45.0f;
	navigation->navigationconfiguration.region_min_size			= 50.0f;
	navigation->navigationconfiguration.region_merge_size		= 20.0f;
	navigation->navigationconfiguration.edge_max_len			= 12.0f;
	navigation->navigationconfiguration.edge_max_error			= 1.3f;
	navigation->navigationconfiguration.vert_per_poly			= 6.0f;
	navigation->navigationconfiguration.detail_sample_dst		= 6.0f;
	navigation->navigationconfiguration.detail_sample_max_error	= 1.0f;	
}


unsigned char NAVIGATION_build( NAVIGATION *navigation, const char *obj)
{
	rcHeightfield *rcheightfield;
	
	rcCompactHeightfield *rccompactheightfield;
	
	rcContourSet *rccontourset;

	rcPolyMesh *rcpolymesh;
	
	rcPolyMeshDetail *rcpolymeshdetail;
	
	// Step 1. Initialize build config.
	rcConfig rcconfig;
	memset( &rcconfig, 0, sizeof( rcConfig ) );
	rcconfig.cs						= navigation->navigationconfiguration.cell_size;
	rcconfig.ch						= navigation->navigationconfiguration.cell_height;
	rcconfig.walkableHeight			= ( int )ceilf ( navigation->navigationconfiguration.agent_height / rcconfig.ch );
	rcconfig.walkableRadius			= ( int )ceilf ( navigation->navigationconfiguration.agent_radius / rcconfig.cs );
	rcconfig.walkableClimb			= ( int )floorf( navigation->navigationconfiguration.agent_max_climb / rcconfig.ch );
	rcconfig.walkableSlopeAngle		= navigation->navigationconfiguration.agent_max_slope;
	rcconfig.minRegionArea			= ( int )rcSqr( navigation->navigationconfiguration.region_min_size );
	rcconfig.mergeRegionArea		= ( int )rcSqr( navigation->navigationconfiguration.region_merge_size );
	rcconfig.maxEdgeLen				= ( int )( navigation->navigationconfiguration.edge_max_len / rcconfig.cs );
	rcconfig.maxSimplificationError = navigation->navigationconfiguration.edge_max_error;
	rcconfig.maxVertsPerPoly		= ( int )navigation->navigationconfiguration.vert_per_poly;
	rcconfig.detailSampleDist		= rcconfig.cs * navigation->navigationconfiguration.detail_sample_dst;
	rcconfig.detailSampleMaxError   = rcconfig.ch * navigation->navigationconfiguration.detail_sample_max_error;
	//
	std::vector<Vector3> vertices;
	std::vector<Vector3Int> indices;
	getSceneManager()->nmCreateObjFile(obj, vertices, indices);
	rcContext rc;
	rcCalcBounds( (float*)&vertices[0],
				  vertices.size(),
				  rcconfig.bmin,
				  rcconfig.bmax );
	
	
	rcCalcGridSize(  rcconfig.bmin,
					 rcconfig.bmax,
					 rcconfig.cs,
					&rcconfig.width,
					&rcconfig.height );


	// Step 2. Rasterize input polygon soup.
	// Allocate voxel heightfield where we rasterize our input data to.
	rcheightfield = rcAllocHeightfield();

	rcCreateHeightfield( &rc, *rcheightfield,
						  rcconfig.width,
						  rcconfig.height,
						  rcconfig.bmin,
						  rcconfig.bmax,
						  rcconfig.cs,
						  rcconfig.ch );


	// Allocate array that can hold triangle area types.
	// If you have multiple meshes you need to process, allocate
	// and array which can hold the max number of triangles you need to process.
	navigation->triangle_flags = new unsigned char[ indices.size() ];
	memset( navigation->triangle_flags, 0, indices.size() * sizeof( unsigned char ) );
	// Find triangles which are walkable based on their slope and rasterize them.
	// If your input data is multiple meshes, you can transform them here, calculate
	// the are type for each of the meshes and rasterize them.
	rcMarkWalkableTriangles(&rc,  
							rcconfig.walkableSlopeAngle,
							(float*)&vertices[0],
							vertices.size(),
							(int*)&indices[0],
							indices.size(),
							navigation->triangle_flags );
	

	rcRasterizeTriangles(&rc,  
						(float*)&vertices[0],
						vertices.size(),
						(int*)&indices[0],
						navigation->triangle_flags,
						indices.size(),
						*rcheightfield,
						rcconfig.walkableClimb );


	delete []navigation->triangle_flags;
	navigation->triangle_flags = NULL;	

	// Step 3. Filter walkables surfaces.
	// Once all geoemtry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
	rcFilterLowHangingWalkableObstacles(&rc,   
										rcconfig.walkableClimb,
										*rcheightfield );
	rcFilterLedgeSpans(&rc,   
						rcconfig.walkableHeight,
						rcconfig.walkableClimb,
						*rcheightfield );
	
	rcFilterWalkableLowHeightSpans(&rc,   
									rcconfig.walkableHeight,
									*rcheightfield );

	// Step 4. Partition walkable surface to simple regions.
	// Compact the heightfield so that it is faster to handle from now on.
	// This will result more cache coherent data as well as the neighbours
	// between walkable cells will be calculated.
	rccompactheightfield = rcAllocCompactHeightfield();

	rcBuildCompactHeightfield(&rc,  rcconfig.walkableHeight,
							   rcconfig.walkableClimb,
							   *rcheightfield,
							   *rccompactheightfield );

	rcFreeHeightField( rcheightfield );
	rcheightfield = NULL;

	// Erode the walkable area by agent radius.
	rcErodeWalkableArea(&rc,
				 rcconfig.walkableRadius,
				 *rccompactheightfield );


	// Prepare for region partitioning, by calculating distance field along the walkable surface.
	rcBuildDistanceField(&rc,  *rccompactheightfield );


	// Partition the walkable surface into simple regions without holes.
	rcBuildRegions(&rc,  *rccompactheightfield,
					 0,
					 rcconfig.minRegionArea,
					 rcconfig.mergeRegionArea );


	// Step 5. Trace and simplify region contours.
	rccontourset = rcAllocContourSet();

	rcBuildContours(&rc,  *rccompactheightfield,
					  rcconfig.maxSimplificationError,
					  rcconfig.maxEdgeLen,
					 *rccontourset );


	// Step 6. Build polygons mesh from contours.
	rcpolymesh = rcAllocPolyMesh();
	
	rcBuildPolyMesh(&rc,  *rccontourset,
					  rcconfig.maxVertsPerPoly,
					 *rcpolymesh );


	// Step 7. Create detail mesh which allows to access approximate height on each polygon.
	rcpolymeshdetail = rcAllocPolyMeshDetail();
	rcBuildPolyMeshDetail(&rc,  *rcpolymesh,
						   *rccompactheightfield,
							rcconfig.detailSampleDist,
							rcconfig.detailSampleMaxError,
						   *rcpolymeshdetail );


	rcFreeCompactHeightfield( rccompactheightfield );
	rccompactheightfield = NULL;
	
	rcFreeContourSet( rccontourset );
	rccontourset = NULL;

	int i = 0;
	if( rcconfig.maxVertsPerPoly <= DT_VERTS_PER_POLYGON )
	{
		dtNavMeshCreateParams dtnavmeshcreateparams;
		
		unsigned char *nav_data = NULL;
		
		int nav_data_size = 0;
	
		i = 0;
		while( i != rcpolymesh->npolys )
		{
			if( rcpolymesh->areas[ i ] == RC_WALKABLE_AREA )
			{
				rcpolymesh->areas[ i ] = 0;
				rcpolymesh->flags[ i ] = 0x01;
			}
							
			++i;
		}


		memset( &dtnavmeshcreateparams, 0, sizeof( dtNavMeshCreateParams ) );
		
		dtnavmeshcreateparams.verts			   = rcpolymesh->verts;
		dtnavmeshcreateparams.vertCount		   = rcpolymesh->nverts;
		dtnavmeshcreateparams.polys			   = rcpolymesh->polys;
		dtnavmeshcreateparams.polyAreas		   = rcpolymesh->areas;
		dtnavmeshcreateparams.polyFlags		   = rcpolymesh->flags;
		dtnavmeshcreateparams.polyCount		   = rcpolymesh->npolys;
		dtnavmeshcreateparams.nvp			   = rcpolymesh->nvp;
		
		dtnavmeshcreateparams.detailMeshes	   = rcpolymeshdetail->meshes;
		dtnavmeshcreateparams.detailVerts	   = rcpolymeshdetail->verts;
		dtnavmeshcreateparams.detailVertsCount = rcpolymeshdetail->nverts;
		dtnavmeshcreateparams.detailTris       = rcpolymeshdetail->tris;
		dtnavmeshcreateparams.detailTriCount   = rcpolymeshdetail->ntris;
		
		dtnavmeshcreateparams.walkableHeight   = navigation->navigationconfiguration.agent_height;
		dtnavmeshcreateparams.walkableRadius   = navigation->navigationconfiguration.agent_radius;
		dtnavmeshcreateparams.walkableClimb    = navigation->navigationconfiguration.agent_max_climb;
		
		rcVcopy( dtnavmeshcreateparams.bmin, rcpolymesh->bmin );
		rcVcopy( dtnavmeshcreateparams.bmax, rcpolymesh->bmax );
		
		dtnavmeshcreateparams.cs = rcconfig.cs;
		dtnavmeshcreateparams.ch = rcconfig.ch;
		
		
		dtCreateNavMeshData( &dtnavmeshcreateparams,
							 &nav_data,
							 &nav_data_size );
		
		if( !nav_data ) return 0;
		
		navigation->dtnavmesh = dtAllocNavMesh();
		
		navigation->dtnavmesh->init( nav_data,
									 nav_data_size,
									 DT_TILE_FREE_DATA);
		
		rcFreePolyMesh( rcpolymesh );
		rcpolymesh = NULL;
		
		rcFreePolyMeshDetail( rcpolymeshdetail );
		rcpolymeshdetail = NULL;
		
		return 1;
	}
	/*std::ofstream o("c:\\1.nav", std::ios::binary);
	o.write((char*)navigation, sizeof(u32) * AlphaMapUnCompressed_.size());
	o.close();*/	
	return 0;
}


int NAVIGATION_get_path( NAVIGATION *navigation, NAVIGATIONPATH *navigationpath, NAVIGATIONPATHDATA *navigationpathdata )
{
	Vector3 start_location(navigationpath->start_location.x,
		navigationpath->start_location.y,
		navigationpath->start_location.z);

	Vector3 end_location(navigationpath->end_location.x,
		navigationpath->end_location.y,
		navigationpath->end_location.z );
		
	//vec3_to_recast( &start_location );
								
	//vec3_to_recast( &end_location );
	dtNavMeshQuery q;
	dtStatus ss;
	ss = q.init(navigation->dtnavmesh, NAVIGATION_MAX_NODE);
	if (dtStatusFailed(ss))
	{
		return -1;
	}
	
	ss = q.findNearestPoly( ( float * )&start_location,
		( float * )&navigation->tolerance,
		&navigationpath->path_filter,
		&navigationpath->start_reference,
		0 );
	if (dtStatusFailed(ss))
	{
		return -1;
	}
	ss = q.findNearestPoly(( float * )&end_location,
		( float * )&navigation->tolerance,
		&navigationpath->path_filter,
		&navigationpath->end_reference,
		0);
	if (dtStatusFailed(ss))
	{
		return -1;
	}

	ss = q.findPath(navigationpath->start_reference,
		navigationpath->end_reference,
		( float * )&start_location,
		( float * )&end_location,
		&navigationpath->path_filter,
		navigationpath->poly_array,
		&navigationpath->poly_count,
		NAVIGATION_MAX_POLY
		);

	if (dtStatusFailed(ss))
	{
		return -1;
	}
	if( navigationpath->poly_count )
	{
		Vector3 closest_end;

		if( navigationpath->poly_array[ navigationpath->poly_count - 1 ] != navigationpath->end_reference )
		{
			ss = q.closestPointOnPoly( navigationpath->poly_array[ navigationpath->poly_count - 1 ],
													   ( float * )&end_location,
													   ( float * )&closest_end );
			if (dtStatusFailed(ss))
			{
				return -1;
			}
		}
		else
		{ 
			memcpy( &closest_end, &navigationpath->end_location, sizeof( Vector3 ) );
		}

		ss = q.findStraightPath( ( float * )&start_location,
							( float * )&closest_end,
							navigationpath->poly_array,
							navigationpath->poly_count,
							( float * )navigationpathdata->path_point_array,
							navigationpathdata->path_flags_array,
							navigationpathdata->path_poly_array,
							&navigationpathdata->path_point_count,
							NAVIGATION_MAX_POLY );
		if (dtStatusFailed(ss))
		{
			return -1;
		}
		memcpy( &navigationpathdata->path_point_array[ navigationpathdata->path_point_count ],
				&end_location,
				sizeof( Vector3 ) );
		
	}

	return 0;
}