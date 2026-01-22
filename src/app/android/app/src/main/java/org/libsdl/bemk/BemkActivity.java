package org.libsdl.bemk;

import org.libsdl.app.SDLActivity;
import android.os.Bundle;
import android.content.res.AssetManager;
import java.io.*;

public class BemkActivity extends SDLActivity {

    private static final String FLAG_FILE_NAME = "assets_copied.flag";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Copy assets to external storage if not already copied
        copyAssetsIfNeeded();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        // Necessary evil
        // Since on Android global variables are not cleared when C++ code finishes,
        // we need to fully terminate the whole app.
        System.exit(0);
    }

    private void copyAssetsIfNeeded() {
        File flagFile = new File(getExternalFilesDir(null), FLAG_FILE_NAME);
        if (!flagFile.exists()) {
            copyAssets();
            createFlagFile(flagFile);
        }
    }

    private void copyAssets() {
        AssetManager assetManager = getAssets();
        try {
            // Start copying from the root of the "test" directory
            copyAssetsRecursively(assetManager, "test", getExternalFilesDir(null).getAbsolutePath() + "/test");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void copyAssetsRecursively(AssetManager assetManager, String srcPath, String destPath) {
        try {
            String[] files = assetManager.list(srcPath);
            if (files == null || files.length == 0) {
                // It's a file
                copyFile(assetManager, srcPath, destPath);
            } else {
                // It's a directory
                new File(destPath).mkdirs();
                for (String file : files) {
                    copyAssetsRecursively(assetManager, srcPath + "/" + file, destPath + "/" + file);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void copyFile(AssetManager assetManager, String srcPath, String destPath) {
        InputStream in = null;
        OutputStream out = null;
        try {
            in = assetManager.open(srcPath);
            out = new FileOutputStream(destPath);

            byte[] buffer = new byte[1024];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            in.close();
            out.flush();
            out.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void createFlagFile(File flagFile) {
        try {
            flagFile.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
