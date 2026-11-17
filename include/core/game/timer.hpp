#pragma once

#include <core/libapi.hpp>
#include <core/types.hpp>
#include <core/threading.hpp>
#include <functional>
#include <core/error.hpp>

CORE_DECLARE_NAMESPACE

namespace timer {
	using seconds_t = f32;
	using ticks_t = u16;

	enum ETimeType : u8 {
		//Ticks (every frame)
		TIME_TICKS = 0,
		//Every second (100? ticks)
		TIME_SECONDS = 1,
	};

	f32 TimeDelta() {
		return (1/100); //Placeholder, replace soon
	}

	template<typename T>
	struct CTimer {
		ETimeType m_Type;
		T m_Ticks = 0;
		std::function<void()> m_OnComplete = nullptr;

		bool Decrease() {
			if (m_Type == TIME_SECONDS) {
				m_Ticks -= TimeDelta();
				return;
			}
			m_Ticks--;
			return (m_Ticks <= 0);
		}

		CTimer() {
			if (typeid(T) == seconds_t) {
				m_Type = TIME_SECONDS;
			}
			else if (typeid(T) == ticks_t) {
				m_Type = TIME_TICKS;
			}
			else {
				throw Error::CRuntime("Trying to create timer of an invalid type!");
			}
		}

		CTimer(std::function<void()> p_OnComplete) : CTimer() {
			m_OnComplete = p_OnComplete;
			thread t = [&this]() -> void {
				if (Decrease())
					m_OnComplete();
			};
			t.detach();
		}

	};
}

CORE_END_NAMESPACE