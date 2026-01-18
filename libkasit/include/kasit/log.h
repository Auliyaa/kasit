#pragma once

#ifndef NDEBUG
#define KASIT_LOG_DEBUG(m) std::cout << "[kasit::debug] (" << __FILE__ << ":" << __LINE__ << ") " << m << std::endl
#else
#define KASIT_LOG_DEBUG(m)
#endif // NDEBUG