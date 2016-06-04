//	KMidiMixerWindow.h

#include <Window.h>

class BStringView;
class KMidiMixerConsumer;
class BMessageRunner;
class KLevelMeter;


class KMidiMixerWindow
  : public BWindow
{
  public:
	KMidiMixerWindow(BRect frame, uint32 flags);
	virtual ~KMidiMixerWindow();

	virtual void MessageReceived(BMessage* msg);

  protected:

  private:
	KMidiMixerWindow();	//	Not impremented.
	KMidiMixerWindow(const KMidiMixerWindow&);	//	Not impremented.
	KMidiMixerWindow& operator=(const KMidiMixerWindow&);	//	Not impremented.

//	KMidiMixerConsumer* mConsumer;

};
