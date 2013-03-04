#pragma once
#include "ConfigFont.h"
#include "misc/stdHead.h"
#include "render/KeyFrames.h"
#include "render/vector2.h"
#include "render/vector4.h"
class ApiFont_ FlowText
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
	static const int scCapacity = 30;
	size_t Cursor_;
	std::vector<sTextRow> Texts_;
	sKeyFrameSet<float> Kfs_;
};