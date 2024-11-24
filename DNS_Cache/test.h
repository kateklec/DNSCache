#pragma once

#include <thread>
#include <vector>
#include <iostream>
#include <cassert>
#include "DNSCache.h"
#include "log_duration.h"

void two_threads();
void four_threads();
void test_dns_cache_overflow();
void test_singleton();
void test_dns_cache_performance();
void test_dns_cache_performance_with_threads();