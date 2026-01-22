# Android Build Instructions

## Building Using `gradlew` Command

To build the project using the `gradlew` command, follow these steps:

1. Navigate to the Android Project Directory:
   cd android

2. Clean the Project:
   It's a good practice to clean the project before building it:
   ./gradlew clean

3. Assemble the Project:
   Use the `assembleDebug` or `assembleRelease` task to build the project:
   ./gradlew assembleDebug   # For a debug build
   ./gradlew assembleRelease # For a release build

4. Find the APK:
   After the build completes, you can find the generated APK files in the `app/build/outputs/apk/` directory.

## Importing the Project into Android Studio

To import the project into Android Studio, follow these steps:

1. Open Android Studio:
   Launch Android Studio on your computer.

2. Import Project:
   - Select `File` -> `Open...`.
   - Navigate to the `android` directory of your project and click `OK`.

3. Wait for Gradle Sync:
   Android Studio will start syncing the project with Gradle. This may take a few moments. Ensure you have a stable internet connection for downloading dependencies.

4. Build the Project:
   Once the Gradle sync completes, you can build the project by selecting `Build` -> `Make Project` or by clicking the `Play` button to run the app on an emulator or connected device.

5. Run the Project:
   To run the project on an emulator or a physical device, make sure you have an emulator configured or a device connected via USB. Click the `Run` button or select `Run` -> `Run 'app'`.
