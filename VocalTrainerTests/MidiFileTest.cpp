// MidiFileTest.cpp: определяет точку входа для консольного приложения.
//

#include "MidiFile.h"
#include "MidiFileReader.h"
#include "VxFile.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

static constexpr int    MIDI_TPQ = 120;
static constexpr double MIDI_TEMPO = 104.6;
static constexpr int    MIDI_BASE_VELOCITY = 64;
static constexpr int    MIDI_ZERO_CHANNEL_INDEX = 0;
static constexpr int    MIDI_FIRST_CHANNEL_INDEX = 1;
static constexpr int    MIDI_DRUMS_CHANNEL_INDEX = 9;
static constexpr int    MIDI_ZERO_TRACK_INDEX = 0;
static constexpr int    MIDI_FIRST_TRACK_INDEX = 1;
static constexpr int    MIDI_SECOND_TRACK_INDEX = 2;
static constexpr int    MIDI_THIRD_TRACK_INDEX = 3;
static constexpr int    MIDI_FOURTH_TRACK_INDEX = 4;
static constexpr int    MIDI_MELODY[] = { 77, 76, 74, 72, 79, 79, -1 };
static constexpr int    MIDI_MELODY_OCTAVE_LOWER[] = { 65, 64, 62, 62, 67, 67, -1 };
static constexpr double MIDI_RYTHM[] = { 1., 1., 1., 1., 2., 2., -1. };
static constexpr int    MIDI_MELODY_SIMPLE[] = { 77, -1 };
static constexpr int    MIDI_MELODY_LOW_NOTE_SIMPLE[] = { 48, -1 };
static constexpr int    MIDI_MELODY_HIGH_NOTE_SIMPLE[] = { 84, -1 };
static constexpr double MIDI_RYTHM_SIMPLE[] = { 1., -1. };

static constexpr int    MIDI_MELODY_HIGH_RANGE[] = {54, 80, -1};
static constexpr int    MIDI_MELODY_NORMAL_RANGE[] = {66, 70, -1};
static constexpr double MIDI_RYTHM_RANGE[] = {1., 2., -1.};
static constexpr double MIDI_RYTHM_RANGE_MONO[] = { 1., 1., -1. };
static constexpr double MIDI_RYTHM_RANGE_MONO_FAST[] = { 0.1, 0.1, -1. };

static constexpr char   SUBSTRING_VOCAL[] = "1_VocAl";
static constexpr char   SUBSTRING_LEAD[] = "leAd1";
static constexpr char   SUBSTRING_SONG[] = "SONG";
static constexpr char   SUBSTRING_MELODY[] = "melod1";

static constexpr char   MIDI_GENERATION_TEST[] = "MIDI generation test";
static constexpr char   MIDI_READER_TEST[] = "MIDI reading test";

/*!
* \brief addTrack
*
* Adds monophonic melody to midifile
*
* \param midifile
* Midi file writer
* \param melody
* Vector of pitches
* \param rythm
* Vector of values of pitches in quaters
* \param trackId
* ID of track to append melody
* \param channelId
* ID of channel to append melody
* \param velocity
* Velocity of each pitch
* \param tempo
* Tempo of the melody
* \param startTick
* Tick number when the first note starts to play
*/
void addTrack(MidiFile &midifile,
	const int *melody,
	const double *rythm,
	const int trackId = MIDI_ZERO_TRACK_INDEX,
	const int channelId = MIDI_ZERO_CHANNEL_INDEX,
	const char* trackName = "",
	const int velocity = MIDI_BASE_VELOCITY,
	const double tempo = MIDI_TEMPO,
	const int startTick = 0) {
	int actionTick = startTick;
	midifile.addTempo(trackId, actionTick, tempo);
	midifile.addPatchChange(trackId, actionTick, channelId, 40); // 40 = violin patch num
	int i = 0;
	if (trackName != "") {
		midifile.addTrackName(trackId, startTick, trackName);
	}
	while (melody[i] >= 0) {
		midifile.addNoteOn(trackId, actionTick, channelId, melody[i], velocity);
		actionTick += MIDI_TPQ * rythm[i];
		midifile.addNoteOff(trackId, actionTick, channelId, melody[i], velocity);
		++i;
	}
}

/*!
* \brief midiDataEmptyTempoTPQ
*
* Returns midi with 1 empty track, having
* TicksPerQuaterNote = MIDI_TPQ
* Tempo = MIDI_TEMPO
*
* \return
*/
stringstream midiDataEmptyTempoTPQ() {
	MidiFile midifile;
	midifile.setTicksPerQuarterNote(MIDI_TPQ);

	//addTempo(int aTrack, int aTick, double aTempo)
	midifile.addTempo(0, 0, MIDI_TEMPO);
	stringstream s;
	midifile.write(s);
	return s;
}

/*!
* \brief midiDataMonophonic
*
* Returns monophonic track:
* Num of tracks = 1
* Num of channels = 1
* TicksPerQuarterNote = MIDI_TPQ
*
* \return
*/
stringstream midiDataMonophonic() {
	MidiFile midifile;
	midifile.setTicksPerQuarterNote(MIDI_TPQ);
	addTrack(midifile,
		MIDI_MELODY,
		MIDI_RYTHM);
	midifile.sortTracks();
	stringstream s;
	midifile.write(s);
	return s;
}

/*!
* \brief midiDataPolyphonic
*
* Returns polyphonic track (playing the same melody by octaves):
* Num of tracks = 1
* Num of channels = 1
* TicksPerQuarterNote = MIDI_TPQ
*
* \return
*/
stringstream midiDataPolyphonic() {
	MidiFile midifile;
	midifile.setTicksPerQuarterNote(MIDI_TPQ);
	addTrack(midifile,
		MIDI_MELODY,
		MIDI_RYTHM);
	//Octave lower
	addTrack(midifile,
		MIDI_MELODY_OCTAVE_LOWER,
		MIDI_RYTHM);
	midifile.sortTracks();
	stringstream s;
	midifile.write(s);
	return s;
}

/*!
* \brief midiDataDrums
*
* Returns monophonic drums track:
* Num of tracks = 1
* Num of channels = 1
* TicksPerQuarterNote = MIDI_TPQ
* Channel № = 9
*
* \return
*/
stringstream midiDataDrums() {
	MidiFile midifile;
	midifile.setTicksPerQuarterNote(MIDI_TPQ);
	addTrack(midifile,
		MIDI_MELODY,
		MIDI_RYTHM,
		MIDI_ZERO_TRACK_INDEX,
		MIDI_DRUMS_CHANNEL_INDEX);
	//Octave lower
	midifile.sortTracks();
	stringstream s;
	midifile.write(s);
	return s;
}

/*!
* \brief midiDataInvalid
*
* Returns invalid midi track
*
* \return
*/
stringstream midiDataInvalid() {
	stringstream s;
	s << "Not a midi";
	return s;
}

/*!
* \brief midiDataMultitrack
*
* Returns MIDI having 2 tracks,
* First track - monophonic melody, 6 notes
* Second track - monophonic melody, 1 note
*
* \return
*/
stringstream midiDataMultitrack() {
	MidiFile midifile;
	midifile.addTracks(1);
	midifile.setTicksPerQuarterNote(MIDI_TPQ);
	//0 track 0 channel
	addTrack(midifile,
		MIDI_MELODY,
		MIDI_RYTHM,
		MIDI_ZERO_TRACK_INDEX,
		MIDI_ZERO_CHANNEL_INDEX
	);

	// 1 track 1 channel
	addTrack(midifile,
		MIDI_MELODY_SIMPLE,
		MIDI_RYTHM_SIMPLE,
		MIDI_FIRST_TRACK_INDEX,
		MIDI_FIRST_CHANNEL_INDEX);
	midifile.sortTracks();
	stringstream s;
	midifile.write(s);
	return s;
}

/*!
* \brief midiDataMultitrack
*
* Returns MIDI having 2 tracks,
* First track - monophonic melody
* Second track - drums
*
*
* \return
*/
stringstream midiDataMultitrackWithDrums() {
	MidiFile midifile;
	midifile.addTracks(1);
	midifile.setTicksPerQuarterNote(MIDI_TPQ);
	// 0 track 0 channel
	addTrack(midifile,
		MIDI_MELODY,
		MIDI_RYTHM,
		MIDI_ZERO_TRACK_INDEX,
		MIDI_ZERO_CHANNEL_INDEX
	);

	// 1 track 9 (drums)channel
	addTrack(midifile,
		MIDI_MELODY_SIMPLE,
		MIDI_RYTHM_SIMPLE,
		MIDI_FIRST_TRACK_INDEX,
		MIDI_DRUMS_CHANNEL_INDEX);
	midifile.sortTracks();
	stringstream s;
	midifile.write(s);
	return s;
}

/*!
* \brief midiDataLowestNote
*
* Returns MIDI having 2 tracks,
* First track has note lower than average range
* Second track has ordinary note
*
*
* \return
*/
stringstream midiDataLowestNote() {
	MidiFile midifile;
	midifile.addTracks(1);
	midifile.setTicksPerQuarterNote(MIDI_TPQ);
	//0 track 0 channel
	addTrack(midifile,
		MIDI_MELODY_LOW_NOTE_SIMPLE,
		MIDI_RYTHM_SIMPLE,
		MIDI_ZERO_TRACK_INDEX,
		MIDI_ZERO_CHANNEL_INDEX
	);

	// 1 track 1 channel
	addTrack(midifile,
		MIDI_MELODY_SIMPLE,
		MIDI_RYTHM_SIMPLE,
		MIDI_FIRST_TRACK_INDEX,
		MIDI_FIRST_CHANNEL_INDEX);
	midifile.sortTracks();
	stringstream s;
	midifile.write(s);
	return s;
}

/*!
* \brief midiDataHighestNote
*
* Returns MIDI having 2 tracks,
* First track has note higher than average range
* Second track has ordinary note
*
*
* \return
*/
stringstream midiDataHighestNote() {
	MidiFile midifile;
	midifile.addTracks(1);
	midifile.setTicksPerQuarterNote(MIDI_TPQ);
	//0 track 0 channel
	addTrack(midifile,
		MIDI_MELODY_HIGH_NOTE_SIMPLE,
		MIDI_RYTHM_SIMPLE,
		MIDI_ZERO_TRACK_INDEX,
		MIDI_ZERO_CHANNEL_INDEX
	);

	// 1 track 1 channel
	addTrack(midifile,
		MIDI_MELODY_SIMPLE,
		MIDI_RYTHM_SIMPLE,
		MIDI_FIRST_TRACK_INDEX,
		MIDI_FIRST_CHANNEL_INDEX);
	midifile.sortTracks();
	stringstream s;
	midifile.write(s);
	return s;
}

/*!
* \brief midiDataNoteRange
*
* Returns MIDI having 2 tracks,
* First track has note range of 25 semitones, that is more than ordinary note range
* Second track has ordinary note range
*
*
* \return
*/
stringstream midiDataNoteRange() {
	MidiFile midifile;
	midifile.addTracks(1);
	midifile.setTicksPerQuarterNote(MIDI_TPQ);
	//0 track 0 channel
	addTrack(midifile,
		MIDI_MELODY_HIGH_RANGE,
		MIDI_RYTHM_RANGE,
		MIDI_ZERO_TRACK_INDEX,
		MIDI_ZERO_CHANNEL_INDEX
	);

	// 1 track 1 channel
	addTrack(midifile,
		MIDI_MELODY_NORMAL_RANGE,
		MIDI_RYTHM_RANGE,
		MIDI_FIRST_TRACK_INDEX,
		MIDI_FIRST_CHANNEL_INDEX);
	midifile.sortTracks();
	stringstream s;
	midifile.write(s);
	return s;
}

/*!
* \brief midiDataValuesDistribution
*
* Returns MIDI having 2 tracks,
* First track has only quarter values
* Second track has quarter and half values
*
* \return
*/
stringstream midiDataValuesDistribution() {
	MidiFile midifile;
	midifile.addTracks(1);
	midifile.setTicksPerQuarterNote(MIDI_TPQ);
	//0 track 0 channel
	addTrack(midifile,
		MIDI_MELODY_NORMAL_RANGE,
		MIDI_RYTHM_RANGE_MONO,
		MIDI_ZERO_TRACK_INDEX,
		MIDI_ZERO_CHANNEL_INDEX
	);

	// 1 track 1 channel
	addTrack(midifile,
		MIDI_MELODY_NORMAL_RANGE,
		MIDI_RYTHM_RANGE,
		MIDI_FIRST_TRACK_INDEX,
		MIDI_FIRST_CHANNEL_INDEX);
	midifile.sortTracks();
	stringstream s;
	midifile.write(s);
	return s;
}

/*!
* \brief midiDataNotesPerSecond
*
* Returns MIDI having 2 tracks,
* First track has 0.1 * quarter note values
* Second track has quarter note values
*
* \return
*/
stringstream midiDataNotesPerSecond() {
	MidiFile midifile;
	midifile.addTracks(1);
	midifile.setTicksPerQuarterNote(MIDI_TPQ);
	//0 track 0 channel
	addTrack(midifile,
		MIDI_MELODY_NORMAL_RANGE,
		MIDI_RYTHM_RANGE_MONO_FAST,
		MIDI_ZERO_TRACK_INDEX,
		MIDI_ZERO_CHANNEL_INDEX
	);

	// 1 track 1 channel
	addTrack(midifile,
		MIDI_MELODY_NORMAL_RANGE,
		MIDI_RYTHM_RANGE_MONO,
		MIDI_FIRST_TRACK_INDEX,
		MIDI_FIRST_CHANNEL_INDEX);
	midifile.sortTracks();
	stringstream s;
	midifile.write(s);
	return s;
}

/*!
* \brief midiDataVocalSubstring
*
* Returns MIDI having 2 tracks,
* First track contains no name
* Second track contains name "vocal"
* Third track contains name "lead"
* Fourth track contains name "song"
* Fifth track contains name "melod"
* \return
*/
stringstream midiDataVocalSubstring() {
	MidiFile midifile;
	midifile.addTracks(4);
	midifile.setTicksPerQuarterNote(MIDI_TPQ);
	//0 track 0 channel no substrings
	addTrack(midifile,
		MIDI_MELODY_OCTAVE_LOWER,
		MIDI_RYTHM,
		MIDI_ZERO_TRACK_INDEX,
		MIDI_ZERO_CHANNEL_INDEX
	);

	// 1 track 1 channel 
	addTrack(midifile,
		MIDI_MELODY,
		MIDI_RYTHM,
		MIDI_FIRST_TRACK_INDEX,
		MIDI_FIRST_CHANNEL_INDEX,
		SUBSTRING_VOCAL);

	// 2 track 1 channel 
	addTrack(midifile,
		MIDI_MELODY,
		MIDI_RYTHM,
		MIDI_SECOND_TRACK_INDEX,
		MIDI_FIRST_CHANNEL_INDEX,
		SUBSTRING_LEAD);

	// 3 track 0 channel 
	addTrack(midifile,
		MIDI_MELODY,
		MIDI_RYTHM,
		MIDI_THIRD_TRACK_INDEX,
		MIDI_ZERO_CHANNEL_INDEX,
		SUBSTRING_SONG);

	// 4 track 0 channel 
	addTrack(midifile,
		MIDI_MELODY,
		MIDI_RYTHM,
		MIDI_FOURTH_TRACK_INDEX,
		MIDI_ZERO_CHANNEL_INDEX,
		SUBSTRING_MELODY);

	midifile.sortTracks();
	stringstream s;
	midifile.write(s);
	return s;
}

TEST_CASE("MIDI reader unit test") {
	std::stringstream midiBuffer;
	MidiFile midifile;
	MidiFileReader midiReader;
	std::vector<VxFile> outResult;
	double outBeatsPerMinute = 0.0;

	/** Testing beats per minute **/
	SECTION("MIDI BPM read test on empty MIDI file") {
		midiBuffer = midiDataEmptyTempoTPQ();

		SECTION(MIDI_GENERATION_TEST) {
			REQUIRE(midifile.read(midiBuffer) != 0);
			REQUIRE(midifile.getTrackCount() == 1);
		}

		SECTION("MIDI BPM read test") {
			midiReader.read(midiBuffer, &outResult, &outBeatsPerMinute);
			double epsilon = 0.01;
			REQUIRE(fabs(outBeatsPerMinute - MIDI_TEMPO) < epsilon);
		}
	}

	/** Testing simple monophonic MIDI **/
	SECTION("Monophonic MIDI file test") {
		midiBuffer = midiDataMonophonic();

		SECTION(MIDI_GENERATION_TEST) {
			REQUIRE(midifile.read(midiBuffer) != 0);
			REQUIRE(midifile.getTrackCount() == 1);
		}

		SECTION(MIDI_READER_TEST) {
			midiReader.read(midiBuffer, &outResult, &outBeatsPerMinute);
			double epsilon = 0.01;
			REQUIRE(fabs(outBeatsPerMinute - MIDI_TEMPO) < epsilon);
			REQUIRE(!outResult.empty());

			SECTION("MIDI pitches data test") {
				auto pitches = outResult[0].getPitches();
				REQUIRE(pitches.size() == 6);
				for (size_t i = 0; i < pitches.size(); i++) {
					REQUIRE(pitches[i].pitch.getMidiIndex() == MIDI_MELODY[i]);
					// Checking duration of pitches
					REQUIRE(pitches[i].bitsCount == MIDI_RYTHM[i] * MIDI_TPQ);
				}
			}
		}
	}

	/** Testing polyphonic MIDI **/
	SECTION("Polyphonic MIDI file test") {
		midiBuffer = midiDataPolyphonic();

		SECTION(MIDI_GENERATION_TEST) {
			REQUIRE(midifile.read(midiBuffer) != 0);
			REQUIRE(midifile.getTrackCount() == 1);
		}

		SECTION(MIDI_READER_TEST) {
			REQUIRE(outResult.empty());
		}
	}

	/** Testing MIDI having only drums (channel 9) **/
	SECTION("Drums MIDI file test") {
		midiBuffer = midiDataDrums();

		SECTION(MIDI_GENERATION_TEST) {
			REQUIRE(midifile.read(midiBuffer) != 0);
			REQUIRE(midifile.getTrackCount() == 1);
		}

		SECTION(MIDI_READER_TEST) {
			midiReader.read(midiBuffer, &outResult, &outBeatsPerMinute);
			REQUIRE(outResult.empty());
		}
	}

	/** Testing invalid format MIDI **/
	SECTION("Invalid MIDI file test") {
		midiBuffer = midiDataInvalid();

		SECTION(MIDI_GENERATION_TEST) {
			REQUIRE(midifile.read(midiBuffer) == 0);
		}

		SECTION(MIDI_READER_TEST) {
			midiReader.read(midiBuffer, &outResult, &outBeatsPerMinute);
			REQUIRE(outResult.empty());
			REQUIRE(outBeatsPerMinute == 0.0);
		}
	}

	/** Testing reading of multitrack MIDI file **/
	SECTION("Multitrack MIDI file test with 2 tracks, 6-note and 1-note") {
		midiBuffer = midiDataMultitrack();

		SECTION(MIDI_GENERATION_TEST) {
			REQUIRE(midifile.read(midiBuffer) != 0);
			REQUIRE(midifile.getTrackCount() == 2);
		}

		SECTION(MIDI_READER_TEST) {
			midiReader.read(midiBuffer, &outResult, &outBeatsPerMinute);
			REQUIRE(outResult.size() == 2);
			SECTION("Output VxFile size test") {
				REQUIRE(outResult.at(0).getPitches().size() == 6);
				REQUIRE(outResult.at(1).getPitches().size() == 1);
			}
			SECTION("Distance in ticks between last pitch end and track end test") {
				REQUIRE(outResult.at(0).getDistanceInTicksBetweenLastPitchEndAndTrackEnd() == 0);
				REQUIRE(outResult.at(1).getDistanceInTicksBetweenLastPitchEndAndTrackEnd() != 0);
			}
		}
	}

	/** Testing reading of multitrack MIDI file with drums track **/
	SECTION("Multitrack MIDI file test with 2 tracks, second track contains drums channel") {
		midiBuffer = midiDataMultitrackWithDrums();

		SECTION(MIDI_GENERATION_TEST) {
			REQUIRE(midifile.read(midiBuffer) != 0);
			REQUIRE(midifile.getTrackCount() == 2);
		}

		SECTION(MIDI_READER_TEST) {
			midiReader.read(midiBuffer, &outResult, &outBeatsPerMinute);
			REQUIRE(outResult.size() == 1);
			REQUIRE(outResult.at(0).getPitches().size() != 1); // Drums channel has only 1 note
		}
	}
	
	SECTION("Sort tests") {
		/** Testing lowest note sort **/
		SECTION("Lowest note sort test") {
			midiBuffer = midiDataLowestNote();
			SECTION(MIDI_GENERATION_TEST) {
				REQUIRE(midifile.read(midiBuffer) != 0);
				REQUIRE(midifile.getTrackCount() == 2);
			}

			SECTION(MIDI_READER_TEST) {
				midiReader.read(midiBuffer, &outResult, &outBeatsPerMinute);
				REQUIRE(outResult.size() == 2);
				REQUIRE(outResult.at(0).getPitches().size() == 1);
				REQUIRE(outResult.at(1).getPitches().size() == 1);
				REQUIRE(outResult.at(0).getPitches()[0].pitch.getMidiIndex() == MIDI_MELODY_SIMPLE[0]);
				REQUIRE(outResult.at(1).getPitches()[0].pitch.getMidiIndex() == MIDI_MELODY_LOW_NOTE_SIMPLE[0]);
			}
		}

		/** Testing lowest note sort **/
		SECTION("Highest note sort test") {
			midiBuffer = midiDataHighestNote();
			SECTION(MIDI_GENERATION_TEST) {
				REQUIRE(midifile.read(midiBuffer) != 0);
				REQUIRE(midifile.getTrackCount() == 2);
			}

			SECTION(MIDI_READER_TEST) {
				midiReader.read(midiBuffer, &outResult, &outBeatsPerMinute);
				REQUIRE(outResult.size() == 2);
				REQUIRE(outResult.at(0).getPitches().size() == 1);
				REQUIRE(outResult.at(1).getPitches().size() == 1);
				REQUIRE(outResult.at(0).getPitches()[0].pitch.getMidiIndex() == MIDI_MELODY_SIMPLE[0]);
				REQUIRE(outResult.at(1).getPitches()[0].pitch.getMidiIndex() == MIDI_MELODY_HIGH_NOTE_SIMPLE[0]);
			}
		}

		/** Testing range sort **/
		SECTION("Range sort test") {
			midiBuffer = midiDataNoteRange();
			SECTION(MIDI_GENERATION_TEST) {
				REQUIRE(midifile.read(midiBuffer) != 0);
				REQUIRE(midifile.getTrackCount() == 2);
			}

			SECTION(MIDI_READER_TEST) {
				midiReader.read(midiBuffer, &outResult, &outBeatsPerMinute);
				REQUIRE(outResult.size() == 2);
				REQUIRE(outResult.at(0).getPitches().size() == 2);
				REQUIRE(outResult.at(1).getPitches().size() == 2);
				REQUIRE(outResult.at(0).getPitches()[0].pitch.getMidiIndex() == MIDI_MELODY_NORMAL_RANGE[0]);
				REQUIRE(outResult.at(1).getPitches()[0].pitch.getMidiIndex() == MIDI_MELODY_HIGH_RANGE[0]);
			}
		}

		/** Testing rythm sort **/
		SECTION("Rythm sort test") {
			midiBuffer = midiDataValuesDistribution();
			SECTION(MIDI_GENERATION_TEST) {
				REQUIRE(midifile.read(midiBuffer) != 0);
				REQUIRE(midifile.getTrackCount() == 2);
			}

			SECTION(MIDI_READER_TEST) {
				midiReader.read(midiBuffer, &outResult, &outBeatsPerMinute);
				REQUIRE(outResult.size() == 2);
				auto &pitches1 = outResult.at(0).getPitches();
				auto &pitches2 = outResult.at(1).getPitches();
				REQUIRE(pitches1.size() == 2);
				REQUIRE(pitches2.size() == 2);
				for (size_t i = 0; i < pitches1.size(); ++i) {
					REQUIRE(pitches1[i].bitsCount == MIDI_RYTHM_RANGE[i] * MIDI_TPQ);
				}
				for (size_t i = 0; i < pitches2.size(); ++i) {
					REQUIRE(pitches2[i].bitsCount == MIDI_RYTHM_RANGE_MONO[i] * MIDI_TPQ);
				}
			}
		}

		/** Testing notes per second sort **/
		SECTION("Notes per second sort test") {
			midiBuffer = midiDataNotesPerSecond();
			SECTION(MIDI_GENERATION_TEST) {
				REQUIRE(midifile.read(midiBuffer) != 0);
				REQUIRE(midifile.getTrackCount() == 2);
			}

			SECTION(MIDI_READER_TEST) {
				midiReader.read(midiBuffer, &outResult, &outBeatsPerMinute);
				REQUIRE(outResult.size() == 2);
				auto &pitches1 = outResult.at(0).getPitches();
				auto &pitches2 = outResult.at(1).getPitches();
				REQUIRE(pitches1.size() == 2);
				REQUIRE(pitches2.size() == 2);
				for (size_t i = 0; i < pitches1.size(); ++i) {
					REQUIRE(pitches1[i].bitsCount == MIDI_RYTHM_RANGE_MONO[i] * MIDI_TPQ);
				}
				for (size_t i = 0; i < pitches2.size(); ++i) {
					REQUIRE(pitches2[i].bitsCount == MIDI_RYTHM_RANGE_MONO_FAST[i] * MIDI_TPQ);
				}
			}
		}

		/** Testing substring inclusion sort **/
		SECTION("Vocal substing in name test") {
			midiBuffer = midiDataVocalSubstring();
			SECTION(MIDI_GENERATION_TEST) {
				REQUIRE(midifile.read(midiBuffer) != 0);
				REQUIRE(midifile.getTrackCount() == 5);
			}

			SECTION(MIDI_READER_TEST) {
				midiReader.read(midiBuffer, &outResult, &outBeatsPerMinute);
				REQUIRE(outResult.size() == 5);
				auto &pitchesLast = outResult.at(outResult.size() - 1);
				REQUIRE(pitchesLast.getPitches().size() != 0);
				REQUIRE(pitchesLast.getPitches()[0].pitch.getMidiIndex() == MIDI_MELODY_OCTAVE_LOWER[0]);
			}
		}
	}
}