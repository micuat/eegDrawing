#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxJSON.h"
#include "ofxCv.h"

const int width = 640;
const int height = 640;

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void setupVideo();
    void loadFeatMatrix();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    vector<ofVec2f> y;
    vector<ofVec2f> yNew;
    vector<vector<float> > feat_matrix;

    ofxPanel gui;
    ofxFloatSlider lineAlpha;
    ofxFloatSlider stretchRate;
    ofxIntSlider refreshSec;
    ofxIntSlider distThreshold;
    
    ofVec2f sample;
    int sampleIndex;
    
    ofVboMesh stringsNew;
    bool drawGui;
    
    ofFbo fbo;
    
    ofxOscReceiver receiver;
    
    ofVideoPlayer videoPlayer;
    int curFrame;
    
    ofxJSONElement responsex, responsey, responsez;
    vector<ofPoint> points;
    
    bool showVideo;
    bool showPoints;
    bool showSounds;
    int switchFlag;
    
    ofSoundPlayer sound;
    int curSoundIndex;
    vector<ofVec3f> soundTsne;
    
    ofxCv::KalmanPosition kalman;

    ofApp() :
    showVideo(false),
    showPoints(true),
    showSounds(false),
    switchFlag(-1),
    curFrame(0),
    sampleIndex(0),
    curSoundIndex(0) {
    }
};

const string files[] = {"17 Kick 3.wav",
    "1stKick.wav",
    "808 Deep Kick Battery.wav",
    "808 Drop.wav",
    "808 Drop2.wav",
    "808Kick.wav",
    "909SD Battery.wav",
    "909SD Trinity.wav",
    "Ahem hat.wav",
    "Anow SN Rev.wav",
    "Anow SN.wav",
    "ASR BD_Dance_04.wav",
    "ASR BD_Dance_27.wav",
    "ASR BD_Mellow_01.wav",
    "ASR BD_Mellow_29.wav",
    "ASR HH_Close_81.wav",
    "ASR HH_Close_94.wav",
    "ASR HH_Open_37.wav",
    "ASR SN_Dance_13.wav",
    "ASR SN_Dance_14.wav",
    "ASR SN_Dance_14_rev.wav",
    "ASR SN_Mellow_22.wav",
    "ASR SN_Mellow_51.wav",
    "B2M-Kick.wav",
    "BaaaaZiiinnggg Snare.wav",
    "Baazacka! Kick.wav",
    "Bachi 2.wav",
    "BACHI.wav",
    "Bad Breath Ninja Snare.wav",
    "Ballad(Flam).wav",
    "Ballad(Rimshot).wav",
    "Ballad(Single1).wav",
    "Ballad(Single2).wav",
    "Bar perc.wav",
    "Battery HH1.wav",
    "Battery Kick1.wav",
    "Battery Kick2.wav",
    "Battery Kick3.wav",
    "Battery Kick4.wav",
    "Battery Snare1.wav",
    "Battery Synth Snap.wav",
    "Be Nice Kick.wav",
    "BellLouis.wav",
    "BellTree Short-1.wav",
    "BellTree-1.wav",
    "BellTree.wav",
    "Blow it hat.wav",
    "bongo_001a.wav",
    "bongo_001b.wav",
    "bongo_001c.wav",
    "bongo_001d.wav",
    "bongo_001e.wav",
    "bongo_001f.wav",
    "bongo_002a.wav",
    "bongo_002b.wav",
    "bongo_002c.wav",
    "bongo_003a.wav",
    "bongo_003b.wav",
    "bongo_003c.wav",
    "bongo_003d.wav",
    "bongo_004a.wav",
    "bongo_004b.wav",
    "bongo_004c.wav",
    "bongo_005a.wav",
    "bongo_005b.wav",
    "bongo_005c.wav",
    "bongo_005d.wav",
    "bongo_005e.wav",
    "bongo_005f.wav",
    "bongo_005g.wav",
    "bongo_005h.wav",
    "bongo_005i.wav",
    "bongo_005j.wav",
    "bongo_005k.wav",
    "bongo_005l.wav",
    "bongo_005m.wav",
    "bongo_005n.wav",
    "bongo_005o.wav",
    "bongo_005p.wav",
    "bongo_005q.wav",
    "bongo_005r.wav",
    "bongo_005s.wav",
    "bongo_005t.wav",
    "bongo_005u.wav",
    "bongo_005v.wav",
    "bongo_006a.wav",
    "bongo_006b.wav",
    "bongo_006c.wav",
    "bongo_006d.wav",
    "bongo_006e.wav",
    "bongo_006f.wav",
    "bongo_006g.wav",
    "bongo_006h.wav",
    "bongo_006i.wav",
    "bongo_006j.wav",
    "bongo_006k.wav",
    "bongo_006l.wav",
    "bongo_006m.wav",
    "bongo_006n.wav",
    "bongo_006o.wav",
    "bongo_006p.wav",
    "bongo_006q.wav",
    "bongo_006r.wav",
    "bongo_006s.wav",
    "bongo_006t.wav",
    "bongo_006u.wav",
    "bongo_006v.wav",
    "Cabassa1.wav",
    "Cabassa2.wav",
    "Cabassa3.wav",
    "Cabassa4.wav",
    "can_001.wav",
    "can_002.wav",
    "can_003.wav",
    "can_004.wav",
    "can_005.wav",
    "can_006.wav",
    "can_007.wav",
    "can_008.wav",
    "can_009.wav",
    "can_010.wav",
    "can_011.wav",
    "can_012.wav",
    "can_013.wav",
    "can_014.wav",
    "can_015.wav",
    "can_016.wav",
    "can_017.wav",
    "can_018.wav",
    "can_019.wav",
    "can_020.wav",
    "can_021.wav",
    "can_022a.wav",
    "can_022b.wav",
    "Capsule_Kick.wav",
    "Cave Water Drops.wav",
    "Chi.wav",
    "Chikichi_Ah.wav",
    "chime.wav",
    "Chime6.wav",
    "Chime9.wav",
    "CHIMES 1.wav",
    "CHIMES 3.wav",
    "CLACK 2.wav",
    "Clap happy perc.wav",
    "Clap Kick WWRY.wav",
    "Clap.wav",
    "clap_001.wav",
    "clap_002.wav",
    "clap_003.wav",
    "clap_004.wav",
    "clap_005.wav",
    "clap_006.wav",
    "clap_007.wav",
    "clap_008.wav",
    "clap_009.wav",
    "clap_010.wav",
    "clap_011.wav",
    "clap_012.wav",
    "clap_013.wav",
    "clap_014.wav",
    "clap_015.wav",
    "clap_016.wav",
    "clap_017.wav",
    "clap_018.wav",
    "clap_019.wav",
    "clap_020.wav",
    "clap_021.wav",
    "clap_022.wav",
    "clap_023.wav",
    "clap_024.wav",
    "clap_025.wav",
    "clap_026.wav",
    "clap_027.wav",
    "clap_028.wav",
    "clap_029.wav",
    "clap_030.wav",
    "clap_031.wav",
    "clap_032.wav",
    "clap_033.wav",
    "clap_034.wav",
    "clap_035.wav",
    "clap_036.wav",
    "clap_037.wav",
    "clap_038.wav",
    "clap_039.wav",
    "clap_040.wav",
    "clap_041.wav",
    "clap_042.wav",
    "clap_043.wav",
    "clap_044.wav",
    "clap_045.wav",
    "clap_046a.wav",
    "clap_046b.wav",
    "clap_047.wav",
    "claves.wav",
    "Crazy Kick.wav",
    "CYMBAL 1.wav",
    "CYMBAL 5.wav",
    "Cymbal Swell1.wav",
    "Cymbal Swell2.wav",
    "Cymbal Swell3.wav",
    "Cymbal Swell4.wav",
    "CymbalChoke1.wav",
    "CymbalChoke2.wav",
    "CymbalChoke3.wav",
    "CymbalScrape1.wav",
    "CymbalScrape2.wav",
    "CymbalScrape3.wav",
    "CymbalSmash1.wav",
    "CymbalSmash2.wav",
    "CymbalStrike1.wav",
    "CymbalStrike2.wav",
    "DA Hat.wav",
    "DAIKOKU 101.wav",
    "DAT_Clean Clap.wav",
    "E.Tom.wav",
    "Electric Tom.wav",
    //    "EXS Cymbal 1.wav",
    "EXS HHat close.wav",
    "EXS HHat Open.wav",
    "EXS Kick1.wav",
    "F-cym_high_f_plan.wav",
    "F-cym_high_f_side.wav",
    "F-cym_high_p_plan.wav",
    "F-cym_high_p_rub.wav",
    "F-cym_high_p_side.wav",
    "F-cym_low_f_plan.wav",
    "F-cym_low_f_side.wav",
    "F-cym_low_p_plan.wav",
    "F-cym_low_p_rub.wav",
    "F-cym_low_p_side.wav",
    "Feed Backer.wav",
    "Finger Cym Bend.wav",
    "FINGER SNAP 2-M.wav",
    "Finger Snap.wav",
    "FingerHit.wav",
    "fingers_001.wav",
    "Frame Drum0_1.wav",
    "Frame Drum0_2.wav",
    "Frame Drum1_1.wav",
    "Frame Drum1_2.wav",
    "Frame Drum2_1.wav",
    "Frame Drum2_2.wav",
    "Gagaku_D4.wav",
    "Gagaku_Eb4.wav",
    "GONGNOT02X01.wav",
    "Guiro.wav",
    "Hats 10.wav",
    "Hats 3.wav",
    "HiFi-808_Cym.wav",
    "j-cool Crash1.wav",
    "j-cool Crash2.wav",
    //    "j-cool Crash3.wav",
    "j-cool Crash4.wav",
    "j-cool Snare1.wav",
    "j-cool Snare10.wav",
    "j-cool Snare11.wav",
    "j-cool Snare12.wav",
    "j-cool Snare13.wav",
    "j-cool Snare14.wav",
    "j-cool Snare15.wav",
    "j-cool Snare2.wav",
    "j-cool Snare3.wav",
    "j-cool Snare4.wav",
    "j-cool Snare5.wav",
    "j-cool Snare6.wav",
    "j-cool Snare7.wav",
    "j-cool Snare8.wav",
    "j-cool Snare9.wav",
    "JR-Kick.wav",
    "JR-Kick1.wav",
    "JR-Kick10.wav",
    "JR-Kick11.wav",
    "JR-Kick12.wav",
    "JR-Kick12_SCR.wav",
    "JR-Kick13.wav",
    "JR-Kick14.wav",
    "JR-Kick15.wav",
    "JR-Kick16.wav",
    "JR-Kick17.wav",
    "JR-Kick18.wav",
    "JR-Kick19 Rev.wav",
    "JR-Kick19.wav",
    "JR-Kick2.wav",
    "JR-Kick20.wav",
    "JR-Kick21.wav",
    "JR-Kick23.wav",
    "JR-Kick24.wav",
    "JR-Kick25.wav",
    "JR-Kick26.wav",
    "JR-Kick27.wav",
    "JR-Kick3.wav",
    "JR-Kick4.wav",
    "JR-Kick5.wav",
    "JR-Kick6.wav",
    "JR-Kick7.wav",
    "JR-Kick8.wav",
    "JR-Kick9.wav",
    "JV Crash1.wav",
    "JV Hat close.wav",
    "JV Hat open.wav",
    "JV Impact.wav",
    "JV LoFi Snare.wav",
    "JV Orchestral Hit.wav",
    "JV TeknoHit.wav",
    "JV TOM.wav",
    "JVClick1.wav",
    //    "JVClick2.wav",
    "Jx Bass.wav",
    "Katou_SynStab.wav",
    "Kick 1.wav",
    "Kick 3.wav",
    "Kick WWRY.wav",
    "kick.wav",
    "Kick22.wav",
    "kobacchi-kick.wav",
    "Koto Gliss.wav",
    "KOTOR Snare.wav",
    "Ku.wav",
    "Kuchi 1.wav",
    "Legend Kick (ReaL!).wav",
    "Legend Kick.wav",
    "LoFiKick_SCR1.wav",
    "LoFiKick_SCR2.wav",
    "Low Boost.wav",
    "manipulated_cymb_001.wav",
    "manipulated_cymb_002.wav",
    "manipulated_cymb_003.wav",
    "manipulated_cymb_004.wav",
    "manipulated_cymb_005.wav",
    "manipulated_cymb_006.wav",
    "manipulated_cymb_007.wav",
    "manipulated_cymb_008.wav",
    "MARK TREE  1.wav",
    "Meltal Wind.wav",
    "Metal Hit Trinity.wav",
    "MetalShaker1.wav",
    "MetalShaker2.wav",
    "MetalShaker3.wav",
    "Naturual(Rimshot).wav",
    "Naturual(Roll).wav",
    "Naturual(Single1).wav",
    "Naturual(Single2).wav",
    "Ne-Yo_Kick.wav",
    "Neyo-Snare.wav",
    "North Pole Crack.wav",
    "North Pole Hit.wav",
    "North Pole Hit2.wav",
    "North Pole Hit3.wav",
    "North Pole Stream1.wav",
    "NuKick.wav",
    "O-Daiko.wav",
    "OceanDrum10.wav",
    "OceanDrum11.wav",
    "OceanDrum7.wav",
    "OceanDrum9.wav",
    "Ohyagi Kick Rev.wav",
    "Ohyagi Kick.wav",
    "OnlyDreamingClap.wav",
    "OnlyDreamingHHatClosed.wav",
    "OnlyDreamingKick.wav",
    "OnlyDreamingKick2.wav",
    "OnlyDreamingSnare.wav",
    "Oooooohhh Kick.wav",
    "Perc 1.wav",
    "Perc 12.wav",
    "Perc 4.wav",
    "Perc 5.wav",
    "Perc 7.wav",
    "Perc 8.wav",
    "Petbottle_Eb.wav",
    "po_gong_06.wav",
    "PS04_HH.wav",
    "Psyche_Kick1.wav",
    "Psyche_Kick2.wav",
    "Psyche_Kick3.wav",
    "Psyche_Kick4.wav",
    "Psyche_Kick4|_Rev.wav",
    "PT_35 Reverse.wav",
    "R and R Kick.wav",
    "R&B Hit1.wav",
    "R&B Hit2.wav",
    "R&R 2(Flam).wav",
    "R&R 2(Rimshot).wav",
    "R&R 2(Single1).wav",
    "R&R 2(Single2).wav",
    "R&R Snare(Flam).wav",
    "R&R Snare(Rim).wav",
    "R&R Snare(Roll).wav",
    "R&R Snare.wav",
    "RainStick2.wav",
    "Real Hand Clap.wav",
    "Rev Snare1.wav",
    "rimshot_001.wav",
    "rimshot_002.wav",
    "rimshot_003.wav",
    "rimshot_004.wav",
    "rimshot_005.wav",
    "rimshot_006.wav",
    "rimshot_007.wav",
    "rimshot_008.wav",
    "rimshot_009.wav",
    "rimshot_010.wav",
    "rimshot_011.wav",
    "rimshot_012.wav",
    "rimshot_013.wav",
    "rimshot_014.wav",
    "rimshot_015.wav",
    "rimshot_016.wav",
    "rimshot_017.wav",
    "rimshot_018.wav",
    "rimshot_019.wav",
    "rimshot_020.wav",
    "rimshot_021.wav",
    "rimshot_022.wav",
    "rimshot_023.wav",
    "rimshot_024.wav",
    "rimshot_025.wav",
    "rimshot_026.wav",
    "rimshot_027.wav",
    "rimshot_028.wav",
    "rimshot_029.wav",
    "rimshot_030.wav",
    "rimshot_031.wav",
    "rimshot_032.wav",
    "rimshot_033.wav",
    "rimshot_034.wav",
    "rimshot_035.wav",
    "rimshot_036.wav",
    "rimshot_037.wav",
    "rimshot_038.wav",
    "rimshot_039.wav",
    "rimshot_040.wav",
    "rimshot_041.wav",
    "rimshot_042.wav",
    "rimshot_043.wav",
    "rimshot_044.wav",
    "rimshot_045.wav",
    "rimshot_046.wav",
    "rimshot_047.wav",
    "rimshot_048.wav",
    "rimshot_049.wav",
    "rimshot_050.wav",
    "rimshot_051.wav",
    "rimshot_052.wav",
    "rimshot_053.wav",
    "rimshot_054.wav",
    "rimshot_055.wav",
    "rimshot_056.wav",
    "rimshot_057.wav",
    "rimshot_058.wav",
    "rimshot_059.wav",
    "rimshot_060.wav",
    "rimshot_061.wav",
    "rimshot_062.wav",
    "rimshot_063.wav",
    "rimshot_064.wav",
    "rimshot_065.wav",
    "rimshot_066.wav",
    "rimshot_067.wav",
    "rimshot_068.wav",
    "rimshot_069.wav",
    "rimshot_070.wav",
    "rimshot_071.wav",
    "rimshot_072.wav",
    "rimshot_073.wav",
    "rimshot_074.wav",
    "rimshot_075.wav",
    "rimshot_076.wav",
    "rimshot_077.wav",
    "rimshot_078.wav",
    "rimshot_079.wav",
    "rimshot_080.wav",
    "rimshot_081.wav",
    "rimshot_082.wav",
    "rimshot_083.wav",
    "rimshot_084.wav",
    "rimshot_085.wav",
    "rimshot_086.wav",
    "rimshot_087.wav",
    "rimshot_088.wav",
    "rimshot_089.wav",
    "rimshot_090.wav",
    "rimshot_091.wav",
    "rimshot_092.wav",
    "rimshot_093.wav",
    "rimshot_094.wav",
    "Samba_Hit.wav",
    "SC8850 Kick 1.wav",
    "SC8850 Kick 2.wav",
    "SC8850 Snare 1.wav",
    "SeedShellShaker2.wav",
    "SeedShellShaker3.wav",
    "SeedShellShaker4.wav",
    "SeedShellShaker5.wav",
    "SeedShellShaker6.wav",
    "SeedShellShaker7.wav",
    "Shaker again perc.wav",
    "shaker_001.wav",
    "shaker_002.wav",
    "shaker_003.wav",
    "shaker_004.wav",
    "shaker_005.wav",
    "shaker_006.wav",
    "shaker_007.wav",
    "shaker_008.wav",
    "shaker_009.wav",
    "shaker_010.wav",
    "shaker_011.wav",
    "shaker_012.wav",
    "shaker_013.wav",
    "shaker_014.wav",
    "shaker_015.wav",
    "SHiNTA_Kick1.wav",
    "SHiNTA_Kick2.wav",
    "SHiNTA_Kick3.wav",
    "Shishiodoshi.wav",
    "Skahn.wav",
    "Slam(Flam).wav",
    "Slam(Rimshot).wav",
    "Slam(Single1).wav",
    "Slam(Single2).wav",
    "Sleigh Bells.wav",
    "Snare 1.wav",
    "Snare 8.wav",
    "Snare Trinity.wav",
    "Snare808_Wired.wav",
    "Spectrum Hit Trinity.wav",
    "Spit Up Snare.wav",
    "Splash Cymbal Battery.wav",
    "Square Bend.wav",
    "STAY_SNARE.wav",
    "stick_001.wav",
    "stick_002.wav",
    "stick_003.wav",
    "stick_004.wav",
    "Strings Stab.wav",
    "STY-Kick1.wav",
    "STY-Kick2.wav",
    "Stylus 808_1.wav",
    "Stylus 808_2.wav",
    "Stylus Clap.wav",
    "Stylus HHat1.wav",
    "Stylus Kick1.wav",
    "Stylus Kick2.wav",
    "Stylus Kick3.wav",
    "Stylus Kick4.wav",
    "Stylus Kick5.wav",
    "Stylus RMX-1 chiki.wav",
    "Stylus Snare 22.wav",
    "Stylus Snare 23.wav",
    "Stylus Snare Hiromi.wav",
    "Stylus Snare01.wav",
    "Stylus Snare02.wav",
    "Stylus Snare03.wav",
    "Stylus Snare04.wav",
    "Stylus Snare05.wav",
    "Stylus Snare06.wav",
    "Stylus Snare07.wav",
    "Stylus Snare08.wav",
    "Stylus Snare09.wav",
    "Stylus Snare10.wav",
    "Stylus Snare11.wav",
    "Stylus Snare12.wav",
    "Stylus Snare13.wav",
    "Stylus Snare14.wav",
    "Stylus Snare15.wav",
    "Stylus Snare16.wav",
    "Stylus Snare17.wav",
    "Stylus Snare18.wav",
    "Stylus Snare19.wav",
    "Stylus Snare20.wav",
    "Stylus Snare21.wav",
    "Stylus Tree1.wav",
    "Stylus808BD.wav",
    "Stylus909BD.wav",
    "Suzu.wav",
    "Syn Chorus Hit.wav",
    "Synth.wav",
    "tabla_001a.wav",
    "tabla_001b.wav",
    "Tambourine Roll.wav",
    "tambourine_001.wav",
    "tambourine_002.wav",
    "tambourine_003.wav",
    "tambourine_004.wav",
    "tambourine_005.wav",
    "tambourine_006.wav",
    "tambourine_007.wav",
    "tambourine_008.wav",
    "tambourine_009.wav",
    "tambourine_010.wav",
    "tambourine_011.wav",
    "tambourine_012.wav",
    "tambourine_013.wav",
    "tambourine_014.wav",
    "tambourine_015.wav",
    "tambourine_016.wav",
    "tambourine_017.wav",
    "tambourine_018.wav",
    "tambourine_019.wav",
    "tambourine_020.wav",
    "tambourine_021.wav",
    "tambourine_022.wav",
    "tambourine_023.wav",
    "tambourine_024.wav",
    "tambourine_025.wav",
    "tambourine_026.wav",
    "tambourine_027.wav",
    "tambourine_028.wav",
    "tambourine_029.wav",
    "tambourine_030.wav",
    "tambourine_031.wav",
    "tambourine_032.wav",
    "tambourine_033.wav",
    "tambourine_034.wav",
    "tambourine_035.wav",
    "tambourine_036.wav",
    "tambourine_037.wav",
    //    "tambourine_038.wav",
    "tambourine_039.wav",
    "tambourine_040.wav",
    "tambourine_041.wav",
    "tambourine_042.wav",
    "tambourine_043.wav",
    "tambourine_044.wav",
    "tambourine_045.wav",
    "tambourine_046.wav",
    "tambourine_047.wav",
    "tambourine_048.wav",
    "tambourine_049.wav",
    "tambourine_050.wav",
    "tambourine_051.wav",
    "tambourine_052.wav",
    "tambourine_053.wav",
    "tambourine_054.wav",
    "tambourine_055a.wav",
    "tambourine_055b.wav",
    "tambourine_056.wav",
    "tambourine_057.wav",
    "tambourine_058a.wav",
    "tambourine_058b.wav",
    "tambourine_059a.wav",
    "tambourine_059b.wav",
    "tambourine_060a.wav",
    "tambourine_060b.wav",
    "Techno Snare1.wav",
    "Teddy Glass.wav",
    "Teddy SN 1.wav",
    "Teddy SN 2.wav",
    "Teddy SN 3.wav",
    "ThunderTube1.wav",
    "ThunderTube3.wav",
    "ThunderTube6.wav",
    "tom_001.wav",
    "tom_002a.wav",
    "tom_002b.wav",
    "tom_003a.wav",
    "tom_003b.wav",
    "tom_004a.wav",
    "tom_004b.wav",
    "tom_004c.wav",
    "tom_005a.wav",
    "tom_005b.wav",
    "tom_005c.wav",
    "tom_006a.wav",
    "tom_006b.wav",
    "tom_006c.wav",
    "tom_007a.wav",
    "tom_007b.wav",
    "tom_007c.wav",
    "tom_008a.wav",
    "tom_008b.wav",
    "tom_008c.wav",
    "tom_009a.wav",
    "tom_009b.wav",
    "tom_009c.wav",
    "tom_010a.wav",
    "tom_010b.wav",
    "tom_010c.wav",
    "tom_011a.wav",
    "tom_011b.wav",
    "tom_011c.wav",
    "tom_012a.wav",
    "tom_012b.wav",
    "tom_013c.wav",
    "tom_014a.wav",
    "tom_014b.wav",
    "tom_015a.wav",
    "tom_015b.wav",
    "tom_015c.wav",
    "tom_016a.wav",
    "tom_016b.wav",
    "tom_016c.wav",
    "tom_017a.wav",
    "tom_017b.wav",
    "tom_017c.wav",
    "tom_018.wav",
    "tom_019a.wav",
    "tom_019b.wav",
    "tom_019c.wav",
    "tom_020a.wav",
    "tom_020b.wav",
    "tom_020c.wav",
    "tom_021a.wav",
    "tom_022b.wav",
    "tom_022c.wav",
    "tom_023a.wav",
    "tom_023b.wav",
    "tom_023c.wav",
    "tom_024a.wav",
    "tom_025b.wav",
    "tom_026c.wav",
    "tom_027a.wav",
    "tom_027b.wav",
    "tom_027c.wav",
    "tom_028.wav",
    "tom_029a.wav",
    "tom_029b.wav",
    "tom_029c.wav",
    "tom_030a.wav",
    "tom_030b.wav",
    "tom_030c.wav",
    "tom_031a.wav",
    "tom_031b.wav",
    "tom_031c.wav",
    "tom_032a.wav",
    "tom_032b.wav",
    "tom_032c.wav",
    "tom_033a.wav",
    "tom_033b.wav",
    "tom_033c.wav",
    "tom_034a.wav",
    "tom_034b.wav",
    "tom_034c.wav",
    "tom_035.wav",
    "tom_036a.wav",
    "tom_036b.wav",
    "tom_036c.wav",
    "tom_037a.wav",
    "tom_037b.wav",
    "tom_037c.wav",
    "tom_038a.wav",
    "tom_038b.wav",
    "tom_038c.wav",
    "tom_039.wav",
    "tom_040a.wav",
    "tom_040b.wav",
    "tom_040c.wav",
    "tom_041a.wav",
    "tom_041b.wav",
    "tom_041c.wav",
    "tom_042a.wav",
    "tom_042b.wav",
    "tom_042c.wav",
    "tom_043a.wav",
    "tom_043b.wav",
    "tom_043c.wav",
    "Toni Snare.wav",
    "triangle_001.wav",
    "Triangle_roll.wav",
    "Trinity Tom.wav",
    "TRITON BELL.wav",
    "tsuzumi.wav",
    "VEC2 Bassdrums Clubby 062 (edited).wav",
    "VEH2 Percussion - 228.wav",
    "VEH2 Percussion - 229.wav",
    "VEH2 Percussion - 231.wav",
    "VIBRASLAP.wav",
    "Wang Snare.wav",
    "Water Drops.wav",
    "WAVs are Lame Snare.wav",
    "Whislte_C3.wav",
    "Whistle_Long.wav",
    "Whistle_Short.wav",
    "WIND CHIME 1.wav",
    "WIND CHIME 2.wav",
    "WIND CHIME 3.wav",
    "WIND CHIME-L.wav",
    "windchime.wav",
    "yooo.wav",
    "Yusuke Snare.wav",
    "Zap.wav"};
