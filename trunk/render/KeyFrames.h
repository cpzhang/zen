#pragma once
#include "misc/stdHead.h"
#include "Interpolate.h"
struct AnimationTime
{
	AnimationTime()
	{
		start = 0;
		end = 0;
		current = 0;
	}
	void update(float delta)
	{
		if (end <= 0.0001f)
		{
			return;
		}
		current += delta;
		while(current > end)
		{
			current -= end;
		}
	}
	//∫¡√Î
	float start;
	float end;
	float current;
};

#pragma pack( push)
#pragma pack( 1 )

template<class T> 
struct sKeyFrame
{
	sKeyFrame() : time(0)
	{
		time = 0;
	}

	sKeyFrame(u32 time,const T& v)
	{
		this->time = time;
		this->v = v;
	}

	bool operator==(u32 time)
	{
		return this->time == time;
	}

	bool operator<(const sKeyFrame<T>& rhs)
	{
		return this->time < rhs.time;
	}

	u32 time;
	T v;
};
#pragma pack( pop )
template<class T>
inline bool kf_less(const sKeyFrame<T>& kf1, const sKeyFrame<T>& kf2)
{
	return kf1.time < kf2.time;
}

template<class T>
class sKeyFrameSet
{
public:
	typedef std::vector<sKeyFrame<T> >	KeyFrameList;
	KeyFrameList			_keyFrames;
	eInterpolationType		_interpolationType;
	typename KeyFrameList::iterator	_it ;
	/// Static
	bool					_bStatic;
	T						_staticData;
	AnimationTime			_at;
public:
	void clear()
	{
		_keyFrames.clear();
	}
	void setInterpolationType(eInterpolationType interpolationType)
	{
		_interpolationType = interpolationType;
	}

	eInterpolationType getInterpolationType()
	{
		return _interpolationType;
	}

	void setStaticData(const T& data)
	{
		_staticData = data;
		_bStatic = true;
	}

	void addKeyFrame(const sKeyFrame<T>& keyFrame)
	{
		_keyFrames.push_back(keyFrame);
		_bStatic = false;
		_at.end = keyFrame.time;
	}
	u32 getTotalTime()
	{
		return _at.end;
	}
	void sort()
	{
		if (_keyFrames.size() > 1)
			std::sort(_keyFrames.begin(), _keyFrames.end(), kf_less);
	}

	size_t numKeyFrames() const
	{
		return _keyFrames.size();
	}

	bool empty()
	{
		return _keyFrames.empty();
	}
	T getFrame(const AnimationTime& t) const
	{
		return getFrame(t.current);
	}
	T getFrame(const u32 t) const
	{
		if (_bStatic || _keyFrames.empty())
		{
			return _staticData;
		}
		if (_keyFrames.size() == 1)
		{
			return _keyFrames[0].v;
		}
		size_t size = _keyFrames.size();
		size_t b = 0;
		size_t e = _keyFrames.size() - 1;
		u32 time = t % (u32)_at.end;
		if (time == b)
		{
			return _keyFrames[b].v;
		}
		if (time == e)
		{
			return _keyFrames[e].v;
		}
		//∂˛∑÷≤È’“
		while(b < e)
		{
			int middle = ((b + e) >> 1);
			if (time == middle)
			{
				return _keyFrames[middle].v;
			}
			if (e - b == 1)
			{
				break;
			}
			if(time > _keyFrames[b].time && time < _keyFrames[middle].time)
			{
				e = middle;
			}
			else
			{
				b = middle;
			}
		}
		float r = (float)(time - _keyFrames[b].time) / (float)(_keyFrames[e].time - _keyFrames[b].time);
		T d;
		switch(_interpolationType)
		{
		case eInterpolationType_Bezier_Linear:
			d = InterpolateBezier(r,_keyFrames[b].v, _keyFrames[e].v);
			break;
		case INTERPOLATION_HERMITE:
//			d = InterpolateHermite(r,_keyFrames[b].v, _keyFrames[e].v);
			break;
		case eInterpolationType_Bezier_Cubic:
//			d = InterpolateBezier(r,_keyFrames[b].v, _keyFrames[e].v);
			break;
		}

		return d;
	}

	const sKeyFrame<T>* getKeyFrame(size_t index) const
	{
		return &_keyFrames[index];
	}
public:
	sKeyFrameSet() : _interpolationType(eInterpolationType_None),_bStatic(true)
	{
	}
};
template<class T>
class KeyFrameController
{
public:
	void update(float delta)
	{
		AniTime_.update(delta);
	}
	T getValue()
	{
		if (KeyFrameSet_)
		{
			KeyFrameSet_->getFrame(AniTime_);
		}
		return DefaultValue_;
	}
public:
	AnimationTime		AniTime_;
	sKeyFrameSet<T>*	KeyFrameSet_;
	T					DefaultValue_;
};