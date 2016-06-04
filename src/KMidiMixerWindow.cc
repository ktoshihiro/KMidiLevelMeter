//	File: KMidiMixerWindow.cc

#include "KMidiMixerWindow.h"

//	System Headers
#include <Roster.h>
//#include <ChannelSlider.h>

//	Project Headers
#include "MidiMixer.h"
#include "KMidiLevelView.h"


KMidiMixerWindow::KMidiMixerWindow(BRect frame, uint32 flags)
  : BWindow(frame, kAppName, B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, flags | B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	KMidiLevelView* aView = new KMidiLevelView(Bounds(), true);
	AddChild(aView);

	SetTitle(aView->Name());

	AddShortcut('N', B_COMMAND_KEY, new BMessage('New '));
	AddShortcut('B', B_COMMAND_KEY, new BMessage('clar'), aView);
//
//
//	BRect aRect(Bounds());
//	aRect.InsetBy(10, 10);
//	aRect.right = aRect.left + 30;
//	BChannelSlider* aSlider = new BChannelSlider(aRect, "name", "label", new BMessage('tehe'));
//	aView->AddChild(aSlider);
}


KMidiMixerWindow::~KMidiMixerWindow()
{
}


void
KMidiMixerWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
	  case 'New ':
		be_roster->Launch(kSignature);
		break;
	  default:
		BWindow::MessageReceived(msg);
		break;
	}
}

