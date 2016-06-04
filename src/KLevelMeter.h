//	KLevelMeter.h

#ifndef __KLevelMeter_h__
#define __KLevelMeter_h__

#include <View.h>

#include <memory>
#include <Bitmap.h>
#include <Region.h>

class KMidiLevelView;


class KLevelMeter
  : public BView
{
  public:
	KLevelMeter(BRect frame, const char* name, bool vertical = true);
	virtual ~KLevelMeter();

	virtual	void AttachedToWindow();
	virtual void Draw(BRect updateRect);
	virtual void MouseDown(BPoint where);

	inline void SetValue(float value);
	inline void Refresh();
	inline void DecrementLevel(float delta);
	inline void DecrementPeak(float delta);

	inline bool IsZero() const;

	enum {
		K_FOREGROUND = 0x00000001,
		K_BACKGROUND = 0x00000002
	};
	void SetBitmap();
	void SetBitmap(const BBitmap* bitmap, int32 selector = 0);

	void Inval();

  protected:

  private:
	KLevelMeter();	//	Not impremented.
	KLevelMeter(const KLevelMeter&);	//	Not impremented.
	KLevelMeter& operator=(const KLevelMeter&);	//	Not impremented.

	bool mIsVertical;

	float mLevel;
	float mPeak;

	auto_ptr<BBitmap> mForegroundBitmap;
	auto_ptr<BBitmap> mBackgroundBitmap;

	BRegion mClipping;
};


void
KLevelMeter::SetValue(float value)
{
	mLevel = value;
	if (mLevel < 0.0f)
		mLevel = 0.0f;
	else if (1.0f < mLevel)
		mLevel = 1.0f;
}


void
KLevelMeter::Refresh()
{
	mLevel = 1.0f;
	mPeak = 1.0f;
}


void
KLevelMeter::DecrementLevel(float delta)
{
	mLevel -= delta;
	if (mLevel < 0.0f)
		mLevel = 0.0f;
}


void
KLevelMeter::DecrementPeak(float delta)
{
	mPeak -= delta;
	if (mPeak < 0.0f)
		mPeak = 0.0f;
}


bool
KLevelMeter::IsZero() const
{
	return ((mLevel == 0.0f) && (mPeak == 0.0f));
}


#endif
