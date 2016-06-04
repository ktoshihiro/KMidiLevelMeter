//	KLevelMeter.cc

#include "KLevelMeter.h"

#include <Debug.h>
#include <Screen.h>
#include <Window.h>

#include "KMidiLevelView.h"


KLevelMeter::KLevelMeter(BRect frame, const char* name, bool vertical)
  : BView(frame, name, B_FOLLOW_NONE, B_WILL_DRAW),
	mIsVertical(vertical),
	mLevel(0.0f),
	mPeak(0.0f),
	mForegroundBitmap(NULL),
	mBackgroundBitmap(NULL)
{
}


KLevelMeter::~KLevelMeter()
{
}


void
KLevelMeter::AttachedToWindow()
{
	SetViewColor(B_TRANSPARENT_COLOR);

	SetHighColor(255, 64, 64);
	SetLowColor(0, 0, 0);

	SetBlendingMode(B_CONSTANT_ALPHA, B_ALPHA_OVERLAY);

	SetPenSize(2.0f);

	SetBitmap();
}


void
KLevelMeter::Draw(BRect updateRect)
{
	BRect aBounds(Bounds());
	float aValue = mLevel;
	float aPeak = mPeak;

#if 0
	static float sLastValue = 0;
	static float sLastPeak = 0;

	clipping_rect aClippingRect;

	GetClippingRegion(&mClipping);

	if (updateRect.IsValid()) {
		aClippingRect.left = updateRect.left;
		aClippingRect.top = updateRect.top;
		aClippingRect.right = updateRect.right;
		aClippingRect.bottom = updateRect.bottom;
		mClipping.Include(aClippingRect);
	}

	float tmpValue = aValue;
	float tmpPeak = aPeak;

	if (sLastValue < tmpValue)
		tmpValue = sLastValue;
	if (tmpPeak < sLastPeak)
		tmpPeak = sLastPeak;

	if (mIsVertical) {
		aClippingRect.left = (int32)rint(aBounds.left);
		aClippingRect.top = (int32)rint(aBounds.top + aBounds.Height() * (1 - tmpPeak) - 1);
		aClippingRect.right = (int32)rint(aBounds.right);
		aClippingRect.bottom = (int32)rint(aBounds.top + aBounds.Height() * (1 - tmpValue) + 1);
	} else {
		aClippingRect.left = (int32)rint(aBounds.right * sLastValue);
		aClippingRect.top = (int32)rint(aBounds.top);
		aClippingRect.right = (int32)rint(aBounds.right * aValue);
		aClippingRect.bottom = (int32)rint(aBounds.bottom);
	}

	mClipping.Include(aClippingRect);
//	mClipping.Set(aClippingRect);
	ConstrainClippingRegion(&mClipping);
//PRINT(("aClippingRect = (%ld, %ld, %ld, %ld)\n", aClippingRect.left, aClippingRect.top, aClippingRect.right, aClippingRect.bottom));
PRINT_OBJECT((mClipping.Frame()));

	sLastValue = aValue;
	sLastPeak = aPeak;
#endif

	Window()->BeginViewTransaction();

	if (mIsVertical) {
		if (mForegroundBitmap.get() != NULL)
			DrawBitmapAsync(mForegroundBitmap.get(), BRect(aBounds.left, aBounds.bottom - aBounds.Height() * aValue, aBounds.right, aBounds.bottom), BRect(aBounds.left, aBounds.bottom - aBounds.Height() * aValue, aBounds.right, aBounds.bottom));
		else
			FillRect(BRect(aBounds.left, aBounds.bottom - aBounds.Height() * aValue, aBounds.right, aBounds.bottom));

		if (aValue != 1.0f) {
			if (mBackgroundBitmap.get() != NULL)
				DrawBitmapAsync(mBackgroundBitmap.get(), BRect(aBounds.left, aBounds.top, aBounds.right, aBounds.bottom - aBounds.Height() * aValue), BRect(aBounds.left, aBounds.top, aBounds.right, aBounds.bottom - aBounds.Height() * aValue));
			else
				FillRect(BRect(aBounds.left, aBounds.top, aBounds.right, aBounds.bottom - aBounds.Height() * aValue), B_SOLID_LOW);
		}

		if (0.0f < aPeak) {
			SetDrawingMode(B_OP_ALPHA);
			SetHighColor(255, 64, 64, 255 * aPeak);
			StrokeLine(BPoint(aBounds.left, aBounds.bottom * (1 - aPeak)), BPoint(aBounds.right, aBounds.bottom * (1 - aPeak)));
			SetHighColor(255, 64, 64, 255);
			SetDrawingMode(B_OP_COPY);
		}
	} else {
		if (mForegroundBitmap.get() != NULL)
			DrawBitmapAsync(mForegroundBitmap.get(), BRect(0, 0, aBounds.right * aValue, aBounds.bottom), BRect(0, 0, aBounds.right * aValue, aBounds.bottom));
		else
			FillRect(BRect(0, 0, aBounds.right * aValue, aBounds.bottom));

		if (aValue != 1.0f) {
			if (mBackgroundBitmap.get() != NULL)
				DrawBitmapAsync(mBackgroundBitmap.get(), BRect(aBounds.right * aValue, 0, aBounds.right, aBounds.bottom), BRect(aBounds.right * aValue, 0, aBounds.right, aBounds.bottom));
			else
				FillRect(BRect(aBounds.right * aValue, 0, aBounds.right, aBounds.bottom), B_SOLID_LOW);
		}

		SetDrawingMode(B_OP_ALPHA);
		SetHighColor(255, 64, 64, 255 * aPeak);
		StrokeLine(BPoint(aBounds.right * aPeak, aBounds.top), BPoint(aBounds.right * aPeak, aBounds.bottom));
		SetHighColor(255, 64, 64, 255);
		SetDrawingMode(B_OP_COPY);
	}

	Window()->EndViewTransaction();
	Flush();
}


void
KLevelMeter::MouseDown(BPoint where)
{
	KMidiLevelView* aView = NULL;

	aView = dynamic_cast<KMidiLevelView*>(Parent());
	if (aView != NULL) {
		aView->MouseDown(ConvertToParent(where));
		return;
	}
}


void
KLevelMeter::SetBitmap()
{
	SetBitmap(NULL, K_FOREGROUND | K_BACKGROUND);
}


void
KLevelMeter::SetBitmap(const BBitmap* bitmap, int32 selector)
{
	if ((selector & (K_FOREGROUND | K_BACKGROUND)) == 0)
		return;

	auto_ptr<BBitmap> srcBitmap(NULL);
	auto_ptr<BBitmap> workBitmap(NULL);

	if (bitmap != NULL) {
		srcBitmap.reset(new BBitmap(bitmap));
	} else {
		if (mIsVertical)
			srcBitmap.reset(new BBitmap(BRect(0, 0, 0, 240 - 16), B_RGB32));
		else
			srcBitmap.reset(new BBitmap(BRect(0, 0, 240 - 16, 0), B_RGB32));

		struct h {
			uchar b;
			uchar g;
			uchar r;
			uchar a;
		};

		h* hh = (h*)srcBitmap->Bits();

		if (mIsVertical)
			hh += srcBitmap->BitsLength() / sizeof (h);

#if 1
//		int Y = 235;
//		int Y = 150;
		int Y = (int32)rint(0.237 * 255 + 16);
		int Cb;
		int Cr;

		Cb = 240;
		for (Cb = 240, Cr = 16; Cr <= 240; /* Cb -= 0.5f, */ Cr++) {
			int r, g, b;
			r = (int32)rint(1.164 * (Y - 16.0) + 1.596 * (Cr - 128.0));
			if (r < 0)
				r = 0;
			else if (255 < r)
				r = 255;
			g = (int32)rint(1.164 * (Y - 16.0) - 0.813 * (Cr - 128.0) + 0.392 * (Cb - 128.0));
			if (g < 0)
				g = 0;
			else if (255 < g)
				g = 255;
			b = (int32)rint(1.164 * (Y - 16.0) + 2.017 * (Cb - 128.0));
			if (b < 0)
				b = 0;
			else if (255 < b)
				b = 255;

			hh->r = r;
			hh->g = g;
			hh->b = b;
			hh->a = 255;
			if (mIsVertical)
				hh--;
			else
				hh++;
		}

//		Cr = 240;
//		for (Cb = 240; 16 <= Cb; Cb--) {
//			hh->r = rint(1.164 * (Y - 16.0) + 1.596 * (Cr - 128.0));
//			hh->g = rint(1.164 * (Y - 16.0) - 0.813 * (Cr - 128.0) + 0.392 * (Cb - 128.0));
//			hh->b = rint(1.164 * (Y - 16.0) + 2.017 * (Cb - 128.0));
//			hh->a = 255;
//			hh++;
//		}
#else
		for (int i = 0; i < 256; i++) {
			hh->b = 255 - i;
			hh->g = i;
			hh->r = 0;
			hh->a = 0;
			hh++;
		}
		for (int i = 0; i < 256; i++) {
			hh->b = 0;
			hh->g = 255 - i;
			hh->r = i;
			hh->a = 0;
			hh++;
		}
#endif
	}

	workBitmap.reset(new BBitmap(Bounds(), BScreen(Window()).ColorSpace(), true));
	BView* aView = NULL;
	if (workBitmap->Lock()) {
		aView = new BView(workBitmap->Bounds(), B_EMPTY_STRING, 0, 0);
		workBitmap->AddChild(aView);
		workBitmap->Unlock();
	}

	if ((selector & K_FOREGROUND) != 0) {
		if (workBitmap->Lock()) {
			aView->DrawBitmap(srcBitmap.get(), aView->Bounds());
			aView->Sync();
			workBitmap->Unlock();
		}

		mForegroundBitmap.reset(new BBitmap(workBitmap.get()));
	}

	if ((selector & K_BACKGROUND) != 0) {
		if (workBitmap->Lock()) {
			aView->PushState();

			aView->DrawBitmapAsync(srcBitmap.get(), aView->Bounds());
			aView->SetDrawingMode(B_OP_BLEND);
			aView->SetHighColor(0, 0, 0);
			aView->FillRect(aView->Bounds());

			aView->PopState();
			aView->Sync();

			workBitmap->Unlock();
		}

		mBackgroundBitmap.reset(new BBitmap(workBitmap.get()));
	}

	Invalidate();
}


void
KLevelMeter::Inval()
{
	static float sLastValue = 0;
	static float sLastPeak = 0;

	BRect aBounds(Bounds());
	float aValue = mLevel;
	float aPeak = mPeak;

	float tmpValue = aValue;
	float tmpPeak = aPeak;

	if (sLastValue < tmpValue)
		tmpValue = sLastValue;
	if (tmpPeak < sLastPeak)
		tmpPeak = sLastPeak;

	if (mIsVertical) {
		aBounds.top = aBounds.top + aBounds.Height() * (1 - tmpPeak) - 2;
		aBounds.bottom = aBounds.top + aBounds.Height() * (1 - tmpValue) + 2;
	} else {
		aBounds.left = (int32)rint(aBounds.right * sLastValue);
		aBounds.right = (int32)rint(aBounds.right * aValue);
	}

	Invalidate(aBounds);

	sLastValue = aValue;
	sLastPeak = aPeak;
}
