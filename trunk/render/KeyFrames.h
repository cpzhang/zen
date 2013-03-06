#pragma once
#include "misc/stdHead.h"
#include "Interpolate.h"
//关键帧动画，时间单位：毫秒
struct AnimationTime
{
	AnimationTime()
	{
		start = 0;
		end = 0;
		current = 0;
		loop = false;
	}
	//终止返回false
	bool update(float delta)
	{
		if (end <= 0.0001f)
		{
			return false;
		}
		current += delta;
		if (current > end)
		{
			if (loop)
			{
				while(current > end)
				{
					current -= end;
				}
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	//毫秒
	float start;
	float end;
	float current;
	bool loop;
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
	//AnimationTime			_at;
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

	const T& getStaticData() const
	{
		return _staticData ;
	}

	void addKeyFrame(const sKeyFrame<T>& keyFrame)
	{
		_keyFrames.push_back(keyFrame);
		_bStatic = false;
		//_at.end = keyFrame.time;
	}
 	u32 getTotalTime()
 	{
		if (_keyFrames.empty())
		{
			return 0;
		}
 		return _keyFrames[_keyFrames.size() - 1].time;
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
		//
		size_t size = _keyFrames.size();
		size_t e = _keyFrames.size() - 1;
		if (t <= _keyFrames[0].time)
		{
			return _keyFrames[0].v;
		}
		if (t >= _keyFrames[e].time)
		{
			return _keyFrames[e].v;
		}
		size_t b = 0;
		//二分查找
		while(b < e)
		{
			int middle = ((b + e) >> 1);
			if (t == _keyFrames[middle].time)
			{
				return _keyFrames[middle].v;
			}
			if (e - b == 1)
			{
				break;
			}
			if(t > _keyFrames[b].time && t < _keyFrames[middle].time)
			{
				e = middle;
			}
			else
			{
				b = middle;
			}
		}
		float r = (float)(t - _keyFrames[b].time) / (float)(_keyFrames[e].time - _keyFrames[b].time);
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
		default:
			{
				d = InterpolateBezier(r,_keyFrames[b].v, _keyFrames[e].v);
				break;
			}
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
	//此处关键帧集合是指针，外面需存在，若Stack临时变量，可能出错
	void init(const AnimationTime& at, sKeyFrameSet<T>* kfs, const T& dv)
	{
		AniTime_ = at;
		KeyFrameSet_ = kfs;
		DefaultValue_ = dv;
		End_ = false;
	}
	void init(sKeyFrameSet<T>* kfs, const T& dv, const bool loop = true)
	{
		AnimationTime at;
		at.start = 0;
		at.current = 0;
		at.loop = loop;
		if (NULL == kfs || kfs->empty())
		{
			at.end = 0;
		}
		else
		{
			at.end = kfs->getKeyFrame(kfs->numKeyFrames() - 1)->time;
		}
		init(at, kfs, dv);
	}
	void clear()
	{
		AniTime_.current = AniTime_.end = 0;
		KeyFrameSet_ = NULL;
	}
	void update(float delta)
	{
		if (End_)
		{
			return;
		}
		if (!AniTime_.update(delta))
		{
			End_ = true;
		}
	}
	T getValue()
	{
		if (KeyFrameSet_)
		{
			if (End_)
			{
				return KeyFrameSet_->getKeyFrame(KeyFrameSet_->numKeyFrames() - 1)->v;
			}
			else
			{
				return KeyFrameSet_->getFrame(AniTime_);
			}
		}
		return DefaultValue_;
	}
	bool empty()
	{
		if (NULL == KeyFrameSet_  || KeyFrameSet_->empty())
		{
			return true;
		}
		return false;
	}
	void begin()
	{
		AniTime_.current = AniTime_.start;
		End_ = false;
	}
public:
	AnimationTime		AniTime_;
	sKeyFrameSet<T>*	KeyFrameSet_;
	T					DefaultValue_;
	bool End_;
};