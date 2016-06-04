//======================================================================
//	File: KMidiMixerConsumer.cc
//======================================================================


#include "KMidiMixerConsumer.h"

#include <AppFileInfo.h>
#include <Bitmap.h>
#include <Message.h>

#include "KMidiLevelView.h"


//======================================================================
#pragma mark === class KMidiMixerConsumer ===
//======================================================================

//----------------------------------------------------------------------

KMidiMixerConsumer::KMidiMixerConsumer(const char* name, KMidiLevelView* owner)
  : BMidiLocalConsumer(name),
	mOwner(owner)
{
	status_t aErr = B_OK;

	BMessage aProperties;
	BAppFileInfo aInfo;
	BBitmap* aLargeIcon = NULL;
	BBitmap* aMiniIcon = NULL;

	aLargeIcon = new BBitmap(BRect(0, 0, 31, 31), B_CMAP8);
	aMiniIcon = new BBitmap(BRect(0, 0, 15, 15), B_CMAP8);

	aErr = aInfo.GetIcon(aLargeIcon, B_LARGE_ICON);
	if (aErr != B_OK)
		BMimeType(B_APP_MIME_TYPE).GetIcon(aLargeIcon, B_LARGE_ICON);

	aErr = aInfo.GetIcon(aMiniIcon, B_MINI_ICON);
	if (aErr != B_OK)
		BMimeType(B_APP_MIME_TYPE).GetIcon(aMiniIcon, B_MINI_ICON);

	GetProperties(&aProperties);

	aErr = aProperties.ReplaceData("be:large_icon", 'ICON', aLargeIcon->Bits(), aLargeIcon->BitsLength());
	if (aErr != B_OK)
		aErr = aProperties.AddData("be:large_icon", 'ICON', aLargeIcon->Bits(), aLargeIcon->BitsLength());

	aErr = aProperties.ReplaceData("be:mini_icon", 'MICN', aMiniIcon->Bits(), aMiniIcon->BitsLength());
	if (aErr != B_OK)
		aErr = aProperties.AddData("be:mini_icon", 'MICN', aMiniIcon->Bits(), aMiniIcon->BitsLength());

	aErr = aProperties.ReplaceString("gopherwood:group_name", mGroupName);
	if (aErr != B_OK)
		aProperties.AddString("gopherwood:group_name", mGroupName);

	SetProperties(&aProperties);

	delete aLargeIcon;
	delete aMiniIcon;
}


//----------------------------------------------------------------------

KMidiMixerConsumer::~KMidiMixerConsumer()
{
}


//----------------------------------------------------------------------

void
KMidiMixerConsumer::NoteOn(uchar channel, uchar note, uchar velocity, bigtime_t time)
{
	if (velocity != 0)
		mOwner->mMeter[channel]->Refresh();
}


//----------------------------------------------------------------------

const BString&
KMidiMixerConsumer::GroupName() const
{
	return mGroupName;
}


//----------------------------------------------------------------------

void
KMidiMixerConsumer::SetGroupName(const BString& name)
{
	mGroupName = name;

	status_t aErr = B_OK;
	BMessage aProperties;

	GetProperties(&aProperties);

	aErr = aProperties.ReplaceString("gopherwood:group_name", mGroupName);
	if (aErr != B_OK)
		aProperties.AddString("gopherwood:group_name", mGroupName);

	SetProperties(&aProperties);
}
