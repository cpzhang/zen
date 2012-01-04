#pragma once
class DeviceCallBack
{
public:
	virtual void deleteUnmanagedObjects( );
	virtual void createUnmanagedObjects( );
	virtual void deleteManagedObjects( );
	virtual void createManagedObjects( );
};