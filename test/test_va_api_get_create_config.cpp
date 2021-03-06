/*
 * Copyright (C) 2016 Intel Corporation. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "test_va_api_get_create_config.h"

namespace VAAPI {
VAAPIGetCreateConfig::VAAPIGetCreateConfig()
{
    m_vaDisplay = doInitialize();
}

VAAPIGetCreateConfig::~VAAPIGetCreateConfig() { doTerminate(); }

TEST_P(VAAPIGetCreateConfig, CreateConfigWithAttributes)
{

    VAProfile currentProfile = ::testing::get<0>(GetParam());
    VAEntrypoint currentEntrypoint = ::testing::get<1>(GetParam());

    doGetMaxValues();

    doQueryConfigProfiles();

    if (doFindProfileInList(currentProfile)) {

        doQueryConfigEntrypoints(currentProfile);
        if (doFindEntrypointInList(currentEntrypoint)) {
            // profile and entrypoint are supported

            doFillConfigAttribList();

            doGetConfigAttributes(currentProfile,
                                                currentEntrypoint);

            doCreateConfigWithAttrib(currentProfile,
                                                   currentEntrypoint);
            doDestroyConfig();
        }
        else {
            // entrypoint is not supported by driver
            doCreateConfigToFail(
                currentProfile, currentEntrypoint,
                VA_STATUS_ERROR_UNSUPPORTED_ENTRYPOINT);
        }
    }
    else {
        // profile is not supported by this driver
        doCreateConfigToFail(currentProfile, currentEntrypoint,
                                           VA_STATUS_ERROR_UNSUPPORTED_PROFILE);
    }
}

TEST_P(VAAPIGetCreateConfig, CreateConfigNoAttributes)
{
    VAProfile currentProfile = ::testing::get<0>(GetParam());
    VAEntrypoint currentEntrypoint = ::testing::get<1>(GetParam());

    doGetMaxValues();

    doQueryConfigProfiles();

    if (doFindProfileInList(currentProfile)) {

        doQueryConfigEntrypoints(currentProfile);
        if (doFindEntrypointInList(currentEntrypoint)) {
            // profile and entrypoint are supported

            doCreateConfigNoAttrib(currentProfile,
                                                 currentEntrypoint);
            doDestroyConfig();
        }
        else {
            // entrypoint is not supported by driver
            doCreateConfigToFail(
                currentProfile, currentEntrypoint,
                VA_STATUS_ERROR_UNSUPPORTED_ENTRYPOINT);
        }
    }
    else {
        // profile is not supported by this driver
        doCreateConfigToFail(currentProfile, currentEntrypoint,
                                           VA_STATUS_ERROR_UNSUPPORTED_PROFILE);
    }
}

TEST_P(VAAPIGetCreateConfig, UnsupportedProfileEntrypoint)
{
    VAProfile currentProfile = ::testing::get<0>(GetParam());
    VAEntrypoint currentEntrypoint = ::testing::get<1>(GetParam());

    doGetMaxValues();

    doQueryConfigProfiles();

    if (!doFindProfileInList(currentProfile)) {
	// non-supported profiles will be skipped from all testing
	doLogSkipTest(currentProfile, currentEntrypoint);
    }
    else {
	doQueryConfigEntrypoints(currentProfile);
	if (!doFindEntrypointInList(currentEntrypoint)) {
	    // non-supported entrypoints will be skipped from all testing
	    doLogSkipTest(currentProfile, currentEntrypoint);
	}
    }
}

INSTANTIATE_TEST_CASE_P(
    GetCreateConfig, VAAPIGetCreateConfig,
    ::testing::Combine(::testing::ValuesIn(m_vaProfiles),
                       ::testing::ValuesIn(m_vaEntrypoints)));

} // VAAPI
