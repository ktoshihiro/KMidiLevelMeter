//	File: KMidiLevelView.cc

#include "KMidiLevelView.h"

//	System Headers
#include <cstdio>
#include <cstdlib>
#include <cstring>
//#include <math.h>
#include <Autolock.h>
#include <Bitmap.h>
#include <Entry.h>
#include <MenuItem.h>
#include <Message.h>
#include <MessageQueue.h>
#include <Messenger.h>
#include <Path.h>
#include <MessageRunner.h>
#include <Messenger.h>
#include <MidiConsumer.h>
#include <MidiProducer.h>
#include <MidiRoster.h>
#include <OS.h>
#include <Screen.h>
#include <TranslationUtils.h>
#include <Window.h>

//	Project Headers
#include "MidiMixer.h"
#include "KMidiMixerConsumer.h"
#include "KLevelMeter.h"
#include "Icons.h"

//	libKT Headers
#include "KDebug.h"
#include "KUtility.h"
#include "KMidiDefs.h"
#include "KMidiRosterAgent.h"

//	Constants
const int kFreq = 30;


//----------------------------------------------------------------------

KMidiLevelView::KMidiLevelView(BRect frame, bool vertical)
  : BView(frame, kAppName, B_FOLLOW_NONE, 0),
	mLock(true),
	mConsumer(NULL),
	mConnection(0),
	mMessageRunner(NULL),
	mPortsMenu(new BPopUpMenu("Ports")),
	mIsDamping(false),
	mDamperID(-1)
{
	BRect aBounds(Bounds());

	const float kGap = 2.0f;

	for (int i = 0; i < 16; i++) {
		BRect aRect(aBounds.Width() / 16 * i, aBounds.top, aBounds.Width() / 16 * (i + 1), aBounds.bottom);
		aRect.InsetBy(kGap / 2, 0);
		mMeter[i] = new KLevelMeter(aRect, B_EMPTY_STRING, vertical);
		AddChild(mMeter[i]);
	}
}


//----------------------------------------------------------------------

KMidiLevelView::~KMidiLevelView()
{
}


//----------------------------------------------------------------------

void
KMidiLevelView::AttachedToWindow()
{
	BString aName(kAppName);

	BMidiConsumer* aConsumer = NULL;
	int aNum = 0;
	int32 aID = 0;

	BMidiRoster* aRoster = BMidiRoster::MidiRoster();
	if (aRoster == NULL)
		return;

	while (true) {
		aConsumer = aRoster->NextConsumer(&aID);
		if (aConsumer == NULL)
			break;

		if (!aConsumer->IsValid()) {
//			aConsumer->Release();
			continue;
		}

		if (strncmp(aConsumer->Name(), aName.String(), aName.Length()) == 0) {
			if (aName.Length() + 1 < strlen(aConsumer->Name())) {
				int tmp = atoi(aConsumer->Name() + aName.Length() + 1);
				if (aNum < tmp)
					aNum = tmp;
			}
		}
//		aConsumer->Release();
	}

	aNum += 1;

	aName << " " << aNum;

	mConsumer = new KMidiMixerConsumer(aName.String(), this);
	mConsumer->SetGroupName(BString(kAppName));
	mConsumer->Register();

	SetName((BString(kAppName) << " " << aNum).String());

//	aRoster->StartWatching(new BMessenger(this));

//	Start();
	mConnection = 0;

	KMidiRosterAgent* aAgent = KMidiRosterAgent::MidiRosterAgent();
	if (aAgent != NULL) {
		StartWatching(aAgent, LIBKT_MIDI_CONNECTED);
		StartWatching(aAgent, LIBKT_MIDI_DISCONNECTED);
	}
}


//----------------------------------------------------------------------

void
KMidiLevelView::DetachedFromWindow()
{
	KMidiRosterAgent* aAgent = KMidiRosterAgent::MidiRosterAgent();
	if (aAgent != NULL)
		StopWatchingAll(aAgent);

	mConsumer->Release();

//	BMidiRoster* aRoster = BMidiRoster::MidiRoster();
//	if (aRoster != NULL)
//		aRoster->StopWatching();

	Stop();
}


//----------------------------------------------------------------------

void
KMidiLevelView::MessageReceived(BMessage* msg)
{
	status_t aErr = B_OK;

	switch (msg->what) {
	  case B_OBSERVER_NOTICE_CHANGE:
		{
			PRINT(("B_OBSERVER_NOTICE_CHANGE\n"));
			PRINT_OBJECT((*msg));

			int32 aChange;

			aErr = msg->FindInt32(B_OBSERVE_WHAT_CHANGE, &aChange);
			if (aErr != B_OK)
				break;

			switch (aChange) {
			  case LIBKT_MIDI_CONNECTED:
				{
					PRINT(("B_OBSERVER_NOTICE_CHANGE -- K_MIDI_CONNECTED\n"));

					KMidiRosterAgent* aAgent = KMidiRosterAgent::MidiRosterAgent();
					if (aAgent == NULL)
						break;

					mConnection = aAgent->CountConnections(mConsumer->ID());
					Start();
				}

			  case LIBKT_MIDI_DISCONNECTED:
				{
					PRINT(("B_OBSERVER_NOTICE_CHANGE -- K_MIDI_DISCONNECTED\n"));

					KMidiRosterAgent* aAgent = KMidiRosterAgent::MidiRosterAgent();
					if (aAgent == NULL)
						break;

					mConnection = aAgent->CountConnections(mConsumer->ID());
					if (0 < mConnection)
						Start();
				}
				break;
			  default:
				break;
			}
		}
		break;

	  case 'clar':
		{
			for (int i = 0; i < 16; i++)
				mMeter[i]->SetBitmap();
		}
		break;
	  case 'bits':
		{
			status_t aErr = B_OK;
			BBitmap* aBitmap = NULL;
			int32 aSelector;

			aErr = msg->FindPointer("bitmap", (void**)&aBitmap);
			if (aErr != B_OK)
				break;
			if (aBitmap == NULL)
				break;
			aErr = msg->FindInt32("selector", &aSelector);
			if (aErr != B_OK)
				aSelector = 0;

			SetBitmap(aBitmap, aSelector);

			delete aBitmap;
		}
		break;

	  case B_SIMPLE_DATA:
		{
			uint32 aModifiers = modifiers();
			uint32 aSelector = 0;

			status_t aErr = B_OK;
			entry_ref aRef;
			BPath aPath;

			aErr = msg->FindRef("refs", &aRef);
			if (aErr != B_OK)
				break;

			aErr = BEntry(&aRef, true).GetPath(&aPath);
			if (aErr != B_OK)
				break;

			BMessage* aMessage = new BMessage;
			aMessage->AddMessenger("replyto", BMessenger(this));
			aMessage->AddRef("refs", &aRef);

			if ((aModifiers & B_COMMAND_KEY) != 0)
				aSelector |= KLevelMeter::K_FOREGROUND;
			if ((aModifiers & B_CONTROL_KEY) != 0)
				aSelector |= KLevelMeter::K_BACKGROUND;

			if (aSelector == 0)
				aSelector = KLevelMeter::K_FOREGROUND | KLevelMeter::K_BACKGROUND;

			aMessage->AddInt32("selector", aSelector);

			thread_id aReaderID = -1;
			aReaderID = spawn_thread((thread_func)reader, "reader", B_NORMAL_PRIORITY, aMessage);
			resume_thread(aReaderID);
		}
		break;

//	  case B_MIDI_EVENT:
//		{
//			BAutolock aLocker(mLock);
//
//			HandleMidiEvent(msg);
//		}
//		break;

//	  case 'on  ':
//		{
//			status_t aErr = B_OK;
//			int32 aChannel;
//
//			aErr = msg->FindInt32("channel", &aChannel);
//			if ((aErr == B_OK) && ((0 <= aChannel) || (aChannel < 16)))
//				mMeter[aChannel]->Refresh();
//
//			BMessageQueue* aQueue = Window()->MessageQueue();
//			if (aQueue->Lock()) {
//				while (true) {
//					BMessage* aMessage = aQueue->FindMessage(uint32('hoge'));
//					if (aMessage == NULL)
//						break;
//					if (!aQueue->RemoveMessage(aMessage))
//						break;;
//					aErr = aMessage->FindInt32("channel", &aChannel);
//					if ((aErr == B_OK) && ((0 <= aChannel) || (aChannel < 16)))
//						mMeter[aChannel]->Refresh();
//					delete aMessage;
//				}
//				aQueue->Unlock();
//			}
//		}
//		break;
//
	  case 'hoge':
		{
			if (Window()->MessageQueue()->FindMessage(uint32('hoge')))
				break;

			{
				BAutolock aLocker(mLock);
				if (mConnection == 0) {
					bool aIsDone = true;
					for (int i = 0; i < 16; i++) {
						if (!(mMeter[i])->IsZero()) {
							aIsDone = false;
							break;
						}
					}
					if (aIsDone) {
						Stop();
						break;
					}
				}
			}

			for (int i = 0; i < 16; i++)
				mMeter[i]->Invalidate();
#if 0
			if (Window()->MessageQueue()->FindMessage(uint32('hoge')))
				mMessageRunner->SetCount(0);
			else
				mMessageRunner->SetCount(-1);
#elif 0
			BMessageQueue* aQueue = Window()->MessageQueue();
			if (aQueue->Lock()) {
				while (true) {
					BMessage* aMessage = aQueue->FindMessage(uint32('hoge'));
					if (aMessage == NULL)
						break;
					if (!aQueue->RemoveMessage(aMessage))
						break;;
					delete aMessage;
				}
				aQueue->Unlock();
			}
#endif
		}
		break;

	  case LIBKT_MIDI_CONNECT:
		{
			K_DEBUG_MARKER("KMidiLevelView::MessageReceived() -- K_MIDI_CONNECT");
			PRINT_OBJECT((*msg));

			status_t aErr = B_OK;

			int32 aID = 0;
			aErr = msg->FindInt32("be:id", &aID);
			if (aErr != B_OK)
				break;

			BString aType;
			aErr = msg->FindString("be:type", &aType);
			if (aErr != B_OK)
				break;

			KMidiRosterAgent* aAgent = KMidiRosterAgent::MidiRosterAgent();
			if (aAgent == NULL)
				break;

			if (aType == "producer") {
				aAgent->Connect(aID, mConsumer->ID());
#if DO_PRELUDE
//				KMidiUtils::Prelude(aID);
#endif
			}
//			if (aType == "consumer") {
//				aAgent->Connect(mSMFProducer->ID(), aID);
//#if DO_PRELUDE
////				KMidiUtils::Prelude(aID);
//#endif
//			}
			mConnection += 1;
			if (mConnection == 1)
				Start();
		}
		break;

	  case LIBKT_MIDI_DISCONNECT:
		{
			K_DEBUG_MARKER("KMidiLevelView::MessageReceived() -- K_MIDI_CONNECT");
			PRINT_OBJECT((*msg));

			status_t aErr = B_OK;

			int32 aID = 0;
			aErr = msg->FindInt32("be:id", &aID);
			if (aErr != B_OK)
				break;

			BString aType;
			aErr = msg->FindString("be:type", &aType);
			if (aErr != B_OK)
				break;

			KMidiRosterAgent* aAgent = KMidiRosterAgent::MidiRosterAgent();
			if (aAgent == NULL)
				break;

			if (aType == "producer") {
				aAgent->Disconnect(aID, mConsumer->ID());
//				KMidiUtils::Postlude(aID);
			}
//			if (aType == "consumer") {
//				aAgent->Disconnect(mSMFProducer->ID(), aID);
//				KMidiUtils::Postlude(aID);
//			}
			mConnection -= 1;
		}
		break;

//	  case K_MIDI_CONNECT:
//		{
//			status_t aErr = B_OK;
//
//			BMidiRoster* aRoster = BMidiRoster::MidiRoster();
//			if (aRoster == NULL)
//				break;
//
//			int32 aID;
//			aErr = msg->FindInt32("be:id", &aID);
//			if (aErr != B_OK)
//				break;
//
//#if 1
//			KMidiRosterAgent* aAgent = KMidiRosterAgent::MidiRosterAgent();
//
//			if (aAgent->IsConnected(aID, mConsumer->ID())) {
//				aAgent->Disconnect(aID, mConsumer->ID());
//				mConnection -= 1;
//			} else {
//				aAgent->Connect(aID, mConsumer->ID());
//				mConnection += 1;
//				if (mConnection == 1)
//					Start();
//			}
//#else
//			BMidiProducer* aProducer = aRoster->FindProducer(aID);
//			if (aProducer == NULL)
//				break;
//
//			if (aProducer->IsConnected(mConsumer)) {
//				aProducer->Disconnect(mConsumer);
//				mConnection -= 1;
//			} else {
//				aProducer->Connect(mConsumer);
//				mConnection += 1;
//				if (mConnection == 1)
//					Start();
//			}
//
////			aProducer->Release();
//#endif
//		}
//		break;

	  default:
		BView::MessageReceived(msg);
		break;
	}
}


//----------------------------------------------------------------------

void
KMidiLevelView::MouseDown(BPoint where)
{
	status_t aErr = B_OK;

	BWindow* aWindow = Window();
	if (aWindow == NULL)
		return;

	BMessage* aMessage = aWindow->CurrentMessage();
	if (aMessage == NULL)
		return;

	int32 aButtons;
	aErr = aMessage->FindInt32("buttons", &aButtons);
	if (aErr != B_OK)
		return;

	const BPoint kOffset(5, 5);

	ConvertToScreen(&where);

	BPopUpMenu* aPopUpMenu = NULL;

	switch (aButtons) {
	  case B_PRIMARY_MOUSE_BUTTON:
		UpdatePortsMenu();
		aPopUpMenu = mPortsMenu.get();
		break;
//	  case B_SECONDARY_MOUSE_BUTTON:
//		break;
//	  case B_TERTIARY_MOUSE_BUTTON:
//		break;
	  default:
		break;
	}

	if (aPopUpMenu != NULL)
		aPopUpMenu->Go(where, true, false, BRect(where - kOffset, where + kOffset), true);

//	The BView version of MouseDown() is empty.
//	BView::MouseDown();
}


//----------------------------------------------------------------------

status_t
KMidiLevelView::SetBitmap(const BBitmap* bitmap, int32 selector)
{
	if (bitmap == NULL)
		return B_ERROR;

	BRect aBounds(Bounds().OffsetToCopy(B_ORIGIN));

	BBitmap* aScaled = NULL;

	if (bitmap->Bounds().Height() < aBounds.Height() && bitmap->Bounds().Width() < aBounds.Width())
		aScaled = new BBitmap(bitmap);
	else {
		float aScaleX = aBounds.Width() / bitmap->Bounds().Width();
		float aScaleY = aBounds.Height() / bitmap->Bounds().Height();
		float aScale = aScaleX < aScaleY ? aScaleX : aScaleY;

		aScaled = new BBitmap(BRect(0, 0, bitmap->Bounds().Width() * aScale, bitmap->Bounds().Height() * aScale), B_RGB32, true);
		if (aScaled->Lock()) {
			BView* aView = new BView(aScaled->Bounds(), B_EMPTY_STRING, B_FOLLOW_NONE, 0);
			aScaled->AddChild(aView);
			aView->DrawBitmap(bitmap, bitmap->Bounds(), aView->Bounds());
			aScaled->Unlock();
		}
	}

	BBitmap* aBitmap = new BBitmap(aBounds, B_RGB32, true);
	if (aBitmap->Lock()) {
		BView* aView = new BView(aBitmap->Bounds(), B_EMPTY_STRING, B_FOLLOW_NONE, 0);
		aBitmap->AddChild(aView);
		for (float y = 0; y < aBounds.bottom; y += aScaled->Bounds().Height()) {
			for (float x = 0; x < aBounds.right; x += aScaled->Bounds().Width()) {
				aView->DrawBitmap(aScaled, BPoint(x, y));
			}
		}
		aBitmap->Unlock();
	}

	delete aScaled;
	aScaled = NULL;

	for (int i = 0; i < 16; i++) {
		BBitmap* tmpBitmap = new BBitmap(mMeter[i]->Bounds().OffsetToCopy(B_ORIGIN), B_RGB32, true);
		if (tmpBitmap->Lock()) {
			BView* aView = new BView(tmpBitmap->Bounds(), B_EMPTY_STRING, B_FOLLOW_NONE, 0);
			tmpBitmap->AddChild(aView);
			aView->DrawBitmap(aBitmap, mMeter[i]->Frame(), aView->Bounds());
			tmpBitmap->Unlock();
		}
		mMeter[i]->SetBitmap(tmpBitmap, selector);
		delete tmpBitmap;
	}

	delete aBitmap;

	return B_OK;
}




//----------------------------------------------------------------------

void
KMidiLevelView::UpdatePortsMenu()
{
	BPopUpMenu* aPortsMenu = NULL;

//	BMidiRoster* aRoster = BMidiRoster::MidiRoster();
//	if (aRoster == NULL)
//		return;
//
	aPortsMenu = new BPopUpMenu("Ports");
	aPortsMenu->SetRadioMode(false);

	KMidiRosterAgent* aAgent = KMidiRosterAgent::MidiRosterAgent();
	if (aAgent == NULL)
		return;

	aAgent->CreateProducerMenu(*aPortsMenu, mConsumer->ID());

	mPortsMenu.reset(aPortsMenu);
}


//----------------------------------------------------------------------

void
KMidiLevelView::Start()
{
	if (mDamperID != -1)
		return;

	if (mMessageRunner == NULL)
		mMessageRunner = new BMessageRunner(BMessenger(this), new BMessage('hoge'), 1000000LL / kFreq);

	if (!mIsDamping) {
		mIsDamping = true;

		mDamperID = spawn_thread((thread_func)damper, "damper", B_NORMAL_PRIORITY, this);
		if (0 < mDamperID) {
			resume_thread(mDamperID);
		} else {
			mDamperID = -1;
			mIsDamping = false;
			return;
		}
	}
}


//----------------------------------------------------------------------

void
KMidiLevelView::Stop()
{
	if (mDamperID == -1)
		return;

	status_t aErr = B_OK;

	mIsDamping = false;
	aErr = wait_for_thread(mDamperID, &aErr);
	mDamperID = -1;

	delete mMessageRunner;
	mMessageRunner = NULL;
}


//----------------------------------------------------------------------

int32
KMidiLevelView::damper(KMidiLevelView* window)
{
	return window->Damper();
}


//----------------------------------------------------------------------

int32
KMidiLevelView::Damper()
{
	bigtime_t aTime = 0LL;
	bigtime_t aDelta = 0LL;

	while (mIsDamping) {
		aDelta = system_time() - aTime;
		aTime = system_time();

		for (int i = 0; i < 16; i++) {
			mMeter[i]->DecrementLevel(aDelta / 500000.0f);
			mMeter[i]->DecrementPeak(aDelta / 4000000.0f);
		}

		snooze(1000000LL / 60);
	}

	return B_OK;
}


//----------------------------------------------------------------------

int32
KMidiLevelView::reader(BMessage* msg)
{
	if (msg == NULL)
		return B_ERROR;

	status_t aErr = B_OK;

	BMessenger aMessenger;
	entry_ref aRef;
	BPath aPath;
	int32 aSelector;
	BBitmap* aBitmap = NULL;
	BMessage aMessage('bits');

	try {
		aErr = msg->FindMessenger("replyto", &aMessenger);
		if (aErr != B_OK)
			throw aErr;

		aErr = msg->FindRef("refs", &aRef);
		if (aErr != B_OK)
			throw aErr;

		aErr = msg->FindInt32("selector", &aSelector);
		if (aErr != B_OK)
			aSelector = 0;

		aErr = BEntry(&aRef, true).GetPath(&aPath);
		if (aErr != B_OK)
			throw aErr;

		aBitmap = BTranslationUtils::GetBitmapFile(aPath.Path());
		if (aBitmap == NULL)
			throw status_t(B_ERROR);

		aErr = aMessage.AddPointer("bitmap", aBitmap);
		if (aErr != B_OK)
			throw aErr;

		aErr = aMessage.AddInt32("selector", aSelector);
		if (aErr != B_OK)
			throw aErr;

		aErr = aMessenger.SendMessage(&aMessage);
		if (aErr != B_OK)
			throw aErr;
	}
	catch (status_t err) {
		aErr = err;
		fprintf(stderr, "*** ERROR: %s\n", strerror(aErr));
		delete aBitmap;
	}

	delete msg;

	return aErr;
}


//void
//KMidiLevelView::HandleMidiEvent(BMessage* msg)
//{
//	status_t aErr = B_OK;
//	int32 aMidiOp = 0;
//
//	aErr = msg->FindInt32("be:op", &aMidiOp);
//	if (aErr != B_OK)
//		return;
//
//	switch (aMidiOp) {
//	  case B_MIDI_NO_OP:
//		break;
//
//	  case B_MIDI_REGISTERED:
//		{
//			PRINT_OBJECT((*msg));
//			BMidiEndpoint* aEndpoint;
//			int32 aID;
//
//			BMidiRoster* aRoster = BMidiRoster::MidiRoster();
//			if (aRoster == NULL)
//				break;
//
//			aErr = msg->FindInt32("be:id", &aID);
//			if (aErr != B_OK)
//				break;
//
//			aEndpoint = aRoster->FindEndpoint(aID);
//			if (aEndpoint == NULL)
//				break;
//
//			PRINT(("Registered: Name = %s\n", aEndpoint->Name()));
//
////			aEndpoint->Release();
//		}
//		break;
//
//	  case B_MIDI_UNREGISTERED:
//		{
//			PRINT_OBJECT((*msg));
//			BMidiEndpoint* aEndpoint;
//			int32 aID;
//
//			BMidiRoster* aRoster = BMidiRoster::MidiRoster();
//			if (aRoster == NULL)
//				break;
//
//			aErr = msg->FindInt32("be:id", &aID);
//			if (aErr != B_OK)
//				break;
//
//			aEndpoint = aRoster->FindEndpoint(aID);
//			if (aEndpoint == NULL)
//				break;
//
//			PRINT(("Unregistered: Name = %s\n", aEndpoint->Name()));
//
////			aEndpoint->Release();
//		}
//		break;
//
//	  case B_MIDI_CONNECTED:
//		{
//			int32 aConsumerID = 0;
//			aErr = msg->FindInt32("be:consumer", &aConsumerID);
//			if (aErr != B_OK)
//				break;
//
//			if (aConsumerID == mConsumer->ID()) {
//				mConnection += 1;
//				if (mConnection == 1)
//					Start();
//				PRINT(("mConnection = %ld\n", mConnection));
//			}
//		}
//		break;
//
//	  case B_MIDI_DISCONNECTED:
//		{
//			int32 aConsumerID = 0;
//			aErr = msg->FindInt32("be:consumer", &aConsumerID);
//			if (aErr != B_OK)
//				break;
//
//			if (aConsumerID == mConsumer->ID()) {
//				mConnection -= 1;
//				PRINT(("mConnection = %ld\n", mConnection));
//			}
//		}
//		break;
//
//	  case B_MIDI_CHANGED_NAME:
//		break;
//
//	  case B_MIDI_CHANGED_LATENCY:
//		break;
//
//	  case B_MIDI_CHANGED_PROPERTIES:
//		break;
//
//	  default:
//		break;
//	}
//}
