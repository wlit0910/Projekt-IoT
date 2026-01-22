# Binka App for IoT project

## Cloning the Repository

To clone the repository along with its submodules, use the following command:
```sh
   git clone --recurse-submodules <repository-url>
```
This ensures all the necessary submodules (SDL and SDL_ttf) are downloaded into the `external` folder, which is required for building the Android and full Linux builds.

## Running the App

1. **Create a Program Folder:**
   - Create a new folder to hold the program files.

2. **Add Executable and Libraries:**
   - Copy the executable file into the newly created folder.
   - Copy the SDL dynamic libraries into the same folder.

3. **Copy Test Folder:**
   - Locate the folder named "test" in the following path: `android/app/src/main/assets`.
   - Copy the "test" folder into the program folder, ensuring it is in the same directory as the executable.
