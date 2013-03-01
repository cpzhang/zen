#pragma once
#include "misc/stdHead.h"
#include "render/KeyFrames.h"
#include "render/vector2.h"
#include "render/vector4.h"
class FlowText
{
public:
	struct sTextRow
	{
		std::string Text_;
		Vector2 Position_;
		Vector4 Color_;
		KeyFrameController<float> AlphaController_;
		void render();
		bool update(float delta);
	private:
		bool isLive_();
	};
	static FlowText* getSingletonP()
	{
		static FlowText ft;
		return &ft;
	}
public:
	FlowText();
	void add(const tstring& t, const Vector4& color);
	void render();
	void update(float delta);
private:
	std::vector<sTextRow> Texts_;
	sKeyFrameSet<float> Kfs_;
};