//======================================================================
//	File: KMidiMixerConsumer.h
//======================================================================

#ifndef __KMidiMixerConsumer_h__
#define __KMidiMixerConsumer_h__

#include <MidiConsumer.h>

//	Forward Declarations
class KMidiLevelView;


//======================================================================
#pragma mark === class KMidiMixerConsumer ===
//======================================================================

class KMidiMixerConsumer
  : public BMidiLocalConsumer
{
  public:
	KMidiMixerConsumer(const char* name, KMidiLevelView* owner);

	virtual	void NoteOn(uchar channel, uchar note, uchar velocity, bigtime_t time);

	const BString& GroupName() const;
	void SetGroupName(const BString& name);

  protected:
	virtual ~KMidiMixerConsumer();

  private:
	KMidiMixerConsumer();	//	Not impremented.
	KMidiMixerConsumer(const KMidiMixerConsumer&);	//	Not impremented.
	KMidiMixerConsumer& operator=(const KMidiMixerConsumer&);	//	Not impremented.

	KMidiLevelView* mOwner;

	BString mGroupName;
};

#endif
