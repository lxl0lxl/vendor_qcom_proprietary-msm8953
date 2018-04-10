// --~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~--
// Copyright 2013-2015 Qualcomm Technologies, Inc.
// All rights reserved.
// Confidential and Proprietary – Qualcomm Technologies, Inc.
// --~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~--
#pragma once

#include <mare/internal/storage/storagemap.hh>

namespace mare {
namespace internal {

int tls_key_create(storage_key*, void (*dtor)(void*));
int tls_set_specific(storage_key, void const*);
void* tls_get_specific(storage_key);

};
};
