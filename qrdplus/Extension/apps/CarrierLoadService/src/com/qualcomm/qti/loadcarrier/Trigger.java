/**
 * Copyright (c) 2014-2016 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

package com.qualcomm.qti.loadcarrier;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map.Entry;

import android.content.Context;
import android.os.AsyncTask;
import android.os.RemoteException;
import android.text.TextUtils;
import android.util.Log;

import com.android.internal.os.RegionalizationEnvironment;
import com.qualcomm.qti.accesscache.ICarrierAccessCacheService;

public abstract class Trigger {
    private static String TAG = "BaseTrigger";
    private static final String CARRIER2DEFAULT_OTA_ZIP_FILE_NAME = "2Default.ota.zip";

    // The path used to find the carriers.
    protected String mCustomerPath = null;
    protected ArrayList<String> mPresetPaths = null;
    protected String mSdcardPath = null;
    protected String mSecSdcardPath = null;

    protected ICarrierAccessCacheService mAccessCacheService = null;
    protected boolean mScanCarriersFinished = false;
    protected HashMap<String, String> mAllCarriers = null;

    protected boolean mIsActive = false;
    protected String mHierarchies = "";

    protected Context mContext = null;
    protected HashMap<String, String> mMatchedCarriers = new HashMap<String, String>();

    interface OnCompleteMatchListener {
        void onCompleteMatch();
    }

    protected OnCompleteMatchListener mListener;

    Trigger(Context context, ICarrierAccessCacheService accessCacheSerivce) {
        mContext = context;
        mAccessCacheService = accessCacheSerivce;
        // Get the path for find the carriers.
        String defPath = context.getString(R.string.trigger_path);
        mCustomerPath = Utils.getValue(Utils.PROP_KEY_TRIGGER_PATH, defPath);
        mPresetPaths = Utils.getRegionalPartitions();
        mSdcardPath = Utils.getPath(mContext, Utils.FLAG_STORAGE_EXTERNAL);
        mSecSdcardPath = Utils.getPath(mContext, Utils.FLAG_STORAGE_SECONDARY);
        if (!mScanCarriersFinished) {
            GetCarriersTask task = new GetCarriersTask(mAccessCacheService);
            task.execute();
        }
    }

    private class GetCarriersTask extends AsyncTask<Void, Void, HashMap<String, String>> {
        private ICarrierAccessCacheService mCacheService = null;

        GetCarriersTask(ICarrierAccessCacheService cacheService) {
            mCacheService = cacheService;
        }

        protected HashMap<String, String> doInBackground(Void... v) {
            HashMap<String, String> carriers = null;
            if (mCacheService != null) {
                try {
                    if (Utils.DEBUG) Log.d(TAG, "Try to get Cache carriers");
                    carriers = (HashMap<String, String>)mCacheService.getCacheCarrierList();
                    if (Utils.DEBUG) Log.d(TAG, "Get Cache carriers done.");
                } catch (RemoteException e) {
                    Log.e(TAG, "Found the LoadCarrierService, but catch RemoteException: "
                            + e.getMessage());
                }
            } else {
                carriers = new HashMap<String, String>();
            }

            if (Utils.isFileBasedSwitchEnabled() || Utils.isPresetMode()) {
                if (Utils.DEBUG) Log.d(TAG, "Try to get Preset carriers");
                for (String path : mPresetPaths) {
                    Utils.getCarrierList(carriers, path, false, null);
                }
                if (Utils.DEBUG) Log.d(TAG, "Get Preset carriers done.");
            }

            if (Utils.DEBUG) Log.d(TAG, "Try to get Secondary SD card carriers");
            Utils.getCarrierList(carriers, mSecSdcardPath);
            if (Utils.DEBUG) Log.d(TAG, "Get Secondary SD card carriers done.");

            return carriers;
        }

        protected void onPostExecute(HashMap<String, String> carriers) {
            if (Utils.DEBUG) {
                for (Entry<String, String> entry : carriers.entrySet()) {
                    String path = entry.getKey();
                    String contents = entry.getValue();
                    Log.d(TAG, "path:" + path + " contents:" + contents);
                }
            }

            mAllCarriers = carriers;
            mScanCarriersFinished = true;
        }
    }

    abstract public void startTrigger();

    abstract public String getHierarchies();

    abstract public boolean isActive();

    abstract public HashMap<String, String> getMatchedCarries();

    protected boolean findCarriersByFilterItems(
            ArrayList<HashMap<String, String>> filterItems) {
        // Try to find the matched carrier, clear it first.
        mMatchedCarriers.clear();

        if (filterItems == null || filterItems.size() < 1)
            return false;

        //Utils.getCarrierList(mMatchedCarriers, mCustomerPath, false,
        //        filterItems);
        if (Utils.isFileBasedSwitchEnabled() || Utils.isPresetMode()) {
            for (String path : mPresetPaths) {
                Utils.getCarrierList(mMatchedCarriers, path, false,
                        filterItems);
            }
        }
        if (!Utils.isPresetMode()) {
            //Utils.getCarrierList(mMatchedCarriers, mSdcardPath, true, filterItems);
            Utils.getCarrierList(mMatchedCarriers, mSecSdcardPath, true, filterItems);
        }

        if (!mScanCarriersFinished && (mAccessCacheService != null)) {
            Log.d(TAG, "Cache has not scanned.");
            try {
                mAllCarriers = (HashMap<String, String>)mAccessCacheService.getCacheCarrierList();
                mScanCarriersFinished = true;
            } catch (RemoteException e) {
                Log.e(TAG, "Found the LoadCarrierService, but catch RemoteException: "
                        + e.getMessage());
            }
        }
        if ((mAllCarriers != null) && !mAllCarriers.isEmpty()) {
            Utils.getMatchedCarrierFromAll(mMatchedCarriers, mAllCarriers, filterItems);
        }

        if (mMatchedCarriers.isEmpty()) {
            return false;
        } else if (mMatchedCarriers.size() == 1) {
            Iterator<Entry<String, String>> iterator = mMatchedCarriers.entrySet().iterator();
            Entry<String, String> carrier = iterator.next();
            String path = carrier.getKey();
            if (path.endsWith(CARRIER2DEFAULT_OTA_ZIP_FILE_NAME)) {
                return false;
            }
        }

        return true;
    }

    protected boolean findPresetCarriersByFilterItems(
            ArrayList<HashMap<String, String>> filterItems) {
        // Try to find the matched carrier, clear it first.
        // mMatchedCarriers.clear();

        if (filterItems == null || filterItems.size() < 1)
            return false;

        for (String path : mPresetPaths) {
            Utils.getCarrierList(mMatchedCarriers, path, false, filterItems);
        }

        if (mMatchedCarriers.isEmpty()) {
            return false;
        } else if (mMatchedCarriers.size() == 1) {
            Iterator<Entry<String, String>> iterator = mMatchedCarriers.entrySet().iterator();
            Entry<String, String> carrier = iterator.next();
            String path = carrier.getKey();
            if (path.endsWith(CARRIER2DEFAULT_OTA_ZIP_FILE_NAME)) {
                return false;
            }
        }

        return true;
    }

    protected int getMatchedCarriersNumber() {
        int matchedCarriersNumber = 0;
        Iterator<Entry<String, String>> iterator = mMatchedCarriers.entrySet().iterator();
        while (iterator.hasNext()) {
            Entry<String, String> carrier = iterator.next();
            String path = carrier.getKey();
            if (!path.endsWith(CARRIER2DEFAULT_OTA_ZIP_FILE_NAME)){
                matchedCarriersNumber++;
            }
        }
        return matchedCarriersNumber;
    }
}
