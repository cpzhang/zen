#include "FlowText.h"
#include "font/FontManager.h"
void FlowText::render()
{
	for (size_t i = 0; i != Texts_.size(); ++i)
	{
		Texts_[i].render();
	}
	//FontManager::getPointer()->getFont()->render();
}

void FlowText::update( float delta )
{
	for (std::vector<sTextRow>::iterator i = Texts_.begin(); i != Texts_.end(); ++i)
	{
		if ((*i).update(delta))
		{
			//++i;
		}
		else
		{
			//i = Texts_.erase(i);
		}
	}
}

void FlowText::add( const tstring& t , const Vector4& color)
{
	sTextRow tr;
	tr.Text_ = t;
	tr.Color_ = color;
	tr.Position_.x = 10;
	tr.Position_.y = Cursor_ * 30 + 10;
	tr.AlphaController_.init(&Kfs_, 1.0f, false);
	Texts_[Cursor_++] = tr;
	if (Cursor_ == scCapacity)
	{
		Cursor_ = 0;
	}
}

FlowText::FlowText()
{
	Kfs_.addKeyFrame(sKeyFrame<float>(0, 1.0f));
	Kfs_.addKeyFrame(sKeyFrame<float>(6000, 0.0f));
	Texts_.resize(scCapacity);
	Cursor_ = 0;
}

void FlowText::sTextRow::render()
{
	if (isLive_())
	{
		FontManager::getPointer()->getFont()->render(Position_, Color_, Text_);
	}
}

bool FlowText::sTextRow::update( float delta )
{
	if (isLive_())
	{
		AlphaController_.update(delta);
		Color_.w = AlphaController_.getValue();
	}
	return isLive_();
}

bool FlowText::sTextRow::isLive_()
{
	return Color_.w >= 0.001f;
}
