//	File: KMidiLevelView.h

#include <View.h>

#include <memory>
#include <Locker.h>
#include <PopUpMenu.h>

#include "KLevelMeter.h"

//class BMidiConsumer;
class KMidiMixerConsumer;


class KMidiLevelView
  : public BView
{
  public:
	KMidiLevelView(BRect frame, bool vertical = true);
	virtual ~KMidiLevelView();

	virtual	void AttachedToWindow();
	virtual void DetachedFromWindow();
	virtual void MessageReceived(BMessage* msg);
	virtual void MouseDown(BPoint where);

	status_t SetBitmap(const BBitmap* bitmap, int32 selector = 0);

  protected:

  private:
	friend class KMidiMixerConsumer;

	KMidiLevelView();	//	Not impremented.
	KMidiLevelView(const KMidiLevelView&);	//	Not impremented.
	KMidiLevelView& operator=(const KMidiLevelView&);	//	Not impremented.

//	void HandleMidiEvent(BMessage* msg);
	void UpdatePortsMenu();

	void Start();
	void Stop();
	static int32 damper(KMidiLevelView* window);
	int32 Damper();

	//	for Graphics reading
	static int32 reader(BMessage* msg);

	BLocker mLock;

	KMidiMixerConsumer* mConsumer;
	int32 mConnection;

	KLevelMeter* mMeter[16];

	BMessageRunner* mMessageRunner;

	auto_ptr<BPopUpMenu> mPortsMenu;

	bool mIsDamping;
	thread_id mDamperID;
};
