# alpha
AOSP Custom "generic" emulator

## Goal
* Provide an AOSP Custom Android Emulator with a Custom System Service (uid=system, persistent=true) to be used by a 3rd Party App to connect the sp<android::hardware::power::V1_0::IPower> proxy to its' hardware service.
* The entire code needs to pass CTS.
* The code needs to compile on Android 9 Pie (PPR2.181005.003.A1).

Full article: https://medium.com/@gilzhaiek/connecting-a-native-hidl-project-treble-to-a-custom-system-service-2194c9ed7e91
