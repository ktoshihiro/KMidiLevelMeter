//	File: MidiMixer.cc

#include <cstdio>
#include <cstdlib>
#include <Application.h>

#include "MidiMixer.h"
#include "KMidiMixerWindow.h"

//	libKT Headers
#include "KMidiRosterAgent.h"


int
main()
{
	status_t aErr = B_OK;

	BApplication aApp(kSignature, &aErr);
	if (aErr != B_OK) {
		fprintf(stderr, "ERROR: BApplication constructor failed. (%s)\n", strerror(aErr));
		fflush(stderr);
		return EXIT_FAILURE;
	}

	aErr = KMidiRosterAgent::InitMidiRosterAgent();
	if (aErr != B_OK) {
		fprintf(stderr, "ERROR: InitMidiRosterAgent failed. (%s)\n", strerror(aErr));
		fflush(stderr);
	}

	BWindow* aWindow = NULL;

	aWindow = new KMidiMixerWindow(BRect(0, 0, 320 - 1, 240 - 1).OffsetToCopy(100, 100), B_QUIT_ON_WINDOW_CLOSE);
	aWindow->Show();

	aApp.Run();

	return EXIT_SUCCESS;
}
