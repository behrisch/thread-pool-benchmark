/*
 * fast_semaphore designed by Joe Seigh, implemented by Chris Thomasson
 *
 * https://www.haiku-os.org/legacy-docs/benewsletter/Issue1-26.html
 */
#pragma once

#include <mutex>
#include <atomic>
#include <condition_variable>
#include <cassert>

class semaphore
{
public:
	explicit semaphore(unsigned int count = 0) noexcept
	: m_count(count) {}

	void post() noexcept
	{
		{
			std::unique_lock lock(m_mutex);
			++m_count;
		}
		m_cv.notify_one();
	}

	void post(unsigned int count) noexcept
	{
		{
			std::unique_lock lock(m_mutex);
			m_count += count;
		}
		m_cv.notify_all();
	}

	void wait() noexcept
	{
		std::unique_lock lock(m_mutex);
		m_cv.wait(lock, [&]() { return m_count != 0; });
		--m_count;
	}

	template<typename T>
	bool wait_for(T&& t) noexcept
	{
		std::unique_lock lock(m_mutex);
		if(!m_cv.wait_for(lock, t, [&]() { return m_count != 0; })) return false;
		--m_count;
		return true;
	}

	template<typename T>
	bool wait_until(T&& t) noexcept
	{
		std::unique_lock lock(m_mutex);
		if(!m_cv.wait_until(lock, t, [&]() { return m_count != 0; })) return false;
		--m_count;
		return true;
	}

private:
	unsigned int m_count;
	std::mutex m_mutex;
	std::condition_variable m_cv;
};

class fast_semaphore
{
public:
	explicit fast_semaphore(int count = 0) noexcept
	: m_count(count), m_semaphore(0) { assert(count > -1); }

	void post() noexcept
	{
		int count = m_count.fetch_add(1, std::memory_order_release);
		if (count < 0)
			m_semaphore.post();
	}

	void wait() noexcept
	{
		int count = m_count.fetch_sub(1, std::memory_order_acquire);
		if (count < 1)
			m_semaphore.wait();
	}

private:
	std::atomic_int m_count;
	semaphore m_semaphore;
};
