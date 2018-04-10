/*
 * Copyright (c) 2015 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
package com.qualcomm.qti.panoramaui.hardware;

import android.app.Activity;
import android.graphics.Point;
import android.graphics.PointF;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.Surface;

import com.qualcomm.qti.panoramaui.AeAfUIHandler;
import com.qualcomm.qti.panoramaui.hardware.AeAfHandler.AeAfCameraListener;

/**
 * This class provides an abstract implementation of a CameraActor. Subclasses can provide the
 * platform-appropriate implementation.
 */
public abstract class CameraActor implements AeAfCameraListener {
    private static final String TAG = "CameraActor";

    private static final float FOCUS_UPPER_CUT = 0.1f;
    private static final float FOCUS_LOWER_CUT = 0.2f;

    /**
     * Classes should implement this interface to be notified when a keyframe capture is complete
     */
    public interface OnKeyFrameCapturedListener {
        /**
         * This method will be invoked after a keyframe capture is complete
         *
         * @param jpegData
         *            JPEG-encoded keyframe
         */
        void onKeyFrameCaptured(byte[] jpegData);
    }

    protected final Activity parentActivity;
    protected final Handler handler;
    protected final AeAfUIHandler aeAfUIHandler;

    /**
     * Construct a new {@link CameraActor}
     *
     * @param parentActivity
     *            {@link Activity} which owns this {@link CameraActor}
     * @throws IllegalArgumentException
     *             if parentActivity is null
     */
    protected CameraActor(Activity parentActivity, AeAfUIHandler aeAfUIHandler) {
        if (parentActivity == null) {
            throw new IllegalArgumentException("parentActivity cannot be null");
        }

        this.parentActivity = parentActivity;
        this.aeAfUIHandler = aeAfUIHandler;
        handler = new Handler(Looper.getMainLooper());
    }

    /**
     * This method initializes the CameraActor, and starts the preview stream
     *
     * @return true if initialized properly, false otherwise
     */
    public abstract boolean startCamera();

    /**
     * clean up method for camera actor
     */
    public abstract void wrapUp();

    /**
     * Returns the width of the camera preview frames (after rotation to match device orientation)
     *
     * @return the width of the camera preview frames, in px
     */
    public abstract int getWidth();

    /**
     * Returns the height of the camera preview frames (after rotation to match device orientation)
     *
     * @return the height of the camera preview frames, in px
     */
    public abstract int getHeight();

    /**
     * Returns the focal length of camera
     *
     * @return the focal length of the camera
     */
    public abstract float getFocalLength();

    /**
     * Lock or unlock AE, AF, and AWB
     *
     * @param start
     *            True to lock AE/AF/AWB, false to unlock
     */
    public abstract void onCapture(boolean start);

    /**
     * Return the camera orientation information
     *
     * @return camera orientation information
     */
    public abstract int getCameraOrientation();

    /**
     * Return boolean true if ZSL is supported, otherwise, false
     *
     * @return boolean true if ZSL is supported
     */
    public abstract boolean isZSLSupported();

    /**
     * Request that a high-resolution key frame be captured, and encoded as JPEG
     *
     * @param jpegCallback
     *            A callback to invoke when the key frame has been captured
     * @throws IllegalArgumentException
     *             if jpegCallback is null
     */
    /* package */abstract void captureKeyFrame(OnKeyFrameCapturedListener jpegCallback);

    /**
     * Calculates the relative orientation between the camera and the display. This is the number of
     * degrees that the camera frames must be rotated (clockwise) to be oriented correctly for
     * display.
     *
     * @param cameraOrientation
     *            Clockwise angle through which camera frames must be rotated to be upright if the
     *            screen is in its native orientation. Must be one of 0, 90, 180, or 270 degrees.
     * @param facingFront
     *            Whether the current camera is front-facing
     * @return the relative orientation between the camera and the display. Will always be one of 0,
     *         90, 180, or 270 degrees.
     */
    protected int getCameraDisplayOrientation(int cameraOrientation, boolean facingFront) {
        int displayRotation;
        switch (parentActivity.getWindowManager().getDefaultDisplay().getRotation()) {
        case Surface.ROTATION_0:
            displayRotation = 0;
            break;
        case Surface.ROTATION_90:
            displayRotation = 90;
            break;
        case Surface.ROTATION_180:
            displayRotation = 180;
            break;
        case Surface.ROTATION_270:
            displayRotation = 270;
            break;
        default:
            throw new RuntimeException("Unexpected rotation value");
        }

        int result;
        if (facingFront) {
            result = (cameraOrientation + displayRotation) % 360;
            result = (360 - result) % 360; // compensate the mirror
        } else { // back-facing
            result = (cameraOrientation - displayRotation + 360) % 360;
        }

        return result;
    }

    /**
     * Given a set of supported preview sizes, and a selected picture size, choose the most
     * appropriate preview size for this device
     *
     * @param supportedSizes
     *            an array of all the supported preview {@link Point}s
     * @param pictureSize
     *            the selected picture size
     * @return one of the {@link Point} values from supportedSizes
     * @throws IllegalArgumentException
     *             if supportedSizes is null or empty, or pictureSize is null
     */
    protected Point selectPreviewSize(Point[] supportedSizes, Point pictureSize) {
        if (supportedSizes == null || supportedSizes.length == 0) {
            throw new IllegalArgumentException("supportedSizes must have length >= 1");
        } else if (pictureSize == null) {
            throw new IllegalArgumentException("pictureSize cannot be null");
        }

        Point displaySize = new Point();
        parentActivity.getWindowManager().getDefaultDisplay().getSize(displaySize);
        final int targetPixels = displaySize.x * displaySize.y;
        final int maxPixels = pictureSize.x * pictureSize.y;

        final float targetAspect = (float) pictureSize.x / (float) pictureSize.y;
        final float targetAspectMin = targetAspect * 0.98f;
        final float targetAspectMax = targetAspect * 1.02f;

        // By default, choose the first supported size. We should always find at least one better
        // match, but this way, we have a fallback.
        Point candidateSize = supportedSizes[0];
        int candidateSizePixelDelta = Integer.MAX_VALUE;

        for (Point size : supportedSizes) {
            // First, eliminate all sizes larger than pictureSize
            final int sizePixels = size.x * size.y;
            if (sizePixels > maxPixels) {
                continue;
            }

            // Second, filter on aspect ratio
            final float aspect = (float) size.x / (float) size.y;
            if (aspect < targetAspectMin || aspect > targetAspectMax) {
                continue;
            }

            // Third, filter on whether this new size is a better match to screen size
            final int sizePixelDelta = Math.abs(targetPixels - sizePixels);
            if (sizePixelDelta > candidateSizePixelDelta) {
                continue;
            }

            // Found a better candidate
            candidateSize = size;
            candidateSizePixelDelta = sizePixelDelta;
        }

        return candidateSize;
    }

    /**
     * check if touch is valid or not
     *
     * @param touchY
     *            touch y axis
     * @param height
     *            height of the camera view
     * @return true if touch is in desired area of focus, false otherwise
     */
    protected boolean isTouchValid(float touchY, int height) {
        float topWipeOut = height * FOCUS_UPPER_CUT;
        float bottomWipeOut = height * FOCUS_LOWER_CUT;
        if (touchY < topWipeOut) {
            Log.d(TAG, "Touch not in desied area: top cut - " + topWipeOut);
            return false;
        } else if (touchY > (height - bottomWipeOut)) {
            Log.d(TAG, "Touch not in desied area: bottom cut - " + bottomWipeOut);
            return false;
        }
        return true;
    }

    /**
     * This method is called when user clicks on a region to get focused
     *
     * @param touch
     *            user touch point
     */
    public abstract void setAeAfRegion(PointF touch);
}
