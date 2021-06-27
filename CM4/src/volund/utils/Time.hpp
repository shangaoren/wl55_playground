#pragma once
#include <cstdint>
#include <array>

namespace volund
{
	namespace utils
	{
		class Time
		{
		public:
			
			static constexpr uint32_t k_minutesToSecundsMultiplicator = 60;
			static constexpr uint32_t k_hoursToSecundsMultiplicator = 60*k_minutesToSecundsMultiplicator;
			static constexpr uint32_t k_daysToSecondsMultiplicator = 24*k_hoursToSecundsMultiplicator;

			
			
			enum class WeekDay : uint8_t
			{
				monday		=	0,
				tuesday		=	1,
				wednesday	=	2,
				thursday	=	3,
				friday		=	4,
				saturday	=	5,
				sunday		=	6,
			};
			
			
class Year
			{
			public: 
				Year(const bool p_isLeap, const uint32_t p_daysSinceEpoch) : isLeap(p_isLeap), daysSinceEpoch(p_daysSinceEpoch)
				{
				}
				const bool isLeap;
				const uint32_t daysSinceEpoch;  // days since 1 january 1970
			};
			
		private:
			
			constexpr static std::size_t k_tableSize = 80;
			//n conversion table to get elapsed days from 1 january 1970 by year and if a year is leap
			// (begin at 2020)
			inline static const Year days2020to2100[k_tableSize] = 
			{
				{ true, 18262 }, 	//2020
				{ false, 18868},	//2021
				{ false, 18993 },	//2022
				{ false, 19358 },	//2023
				{ true, 19723 },	//2024
				{ false, 20089 },	//2025
				{ false, 20454 },	//2026
				{ false, 20819 },	//2027
				{ true, 21184 },	//2028
				{ false, 21550 },	//2029
				{ false, 21915 },	//2030
				{ false, 22280 },	//2031
				{ true, 22645 },	//2032
				{ false, 23011 },	//2033
				{ false, 23376 },	//2034
				{ false, 23741 },	//2035
				{ true, 24106 },	//2036
				{ false, 24472 },	//2037
				{ false, 24837 },	//2038
				{ false, 25202 },	//2039
				{ true, 25567 },	//2040
				{ false, 25933 },	//2041
				{ false, 26298 },	//2042
				{ false, 26663 },	//2043
				{ true, 27028 },	//2044
				{ false, 27394 },	//2045
				{ false, 27759 },	//2046
				{ false, 28124 },	//2047
				{ true, 28489 },	//2048
				{ false, 28855 },	//2049
				{ false, 29220 },	//2050
				{ false, 29585 },	//2051
				{ true, 29950 },	//2052
				{ false, 30316 },	//2053
				{ false, 30681 },	//2054
				{ false, 31046 },	//2055
				{ true, 31411 },	//2056
				{ false, 31777 },	//2057
				{ false, 32142 },	//2058
				{ false, 32507 },	//2059
				{ true, 32872 },	//2060
				{ false, 33238 },	//2061
				{ false, 33603 },	//2062
				{ false, 33968 },	//2063
				{ true, 34333 },	//2064
				{ false, 34699 },	//2065
				{ false, 35064 },	//2066
				{ false, 35429 },	//2067
				{ true, 35794 },	//2068
				{ false, 36160 },	//2069
				{ false, 36525 },	//2070
				{ false, 36890 },	//2071
				{ true, 37255 },	//2072
				{ false, 37621 },	//2073
				{ false, 37986 },	//2074
				{ false, 38351 },	//2075
				{ true, 38716 },	//2076
				{ false, 39082 },	//2077
				{ false, 39447 },	//2078
				{ false, 39812 },	//2079
				{ true, 40177 },	//2080
				{ false, 40543 },	//2081
				{ false, 40908 },	//2082
				{ false, 41273 },	//2083
				{ true, 41638 },	//2084
				{ false, 42004 },	//2085
				{ false, 42369 },	//2086
				{ false, 42734 },	//2087
				{ true, 43099 },	//2088
				{ false, 43465 },	//2089
				{ false, 43830 },	//2090
				{ false, 44195 },	//2091
				{ true, 44560 },	//2092
				{ false, 44926 },	//2093
				{ false, 45291 },	//2094
				{ false, 45656 },	//2095
				{ true, 46021 },	//2096
				{ false, 46387 },	//2097
				{ false, 46752 },	//2098
				{ false, 47117 },	//2099
			};

			// conversion table to get elapsed days in year by month (without leap day)
			inline static const uint16_t daysFromMonthsElapsed[] =
			{
					0, 		//31 january
					31,		//28 february
					59,		//31 march
					90,		//30 april
					120,	//31 may
					151,	//30 june
					181,	//31 july
					212,	//31 august
					243,	//30 september
					273,	//31 ocktober
					304,	//30 november
					334,	//31 december
					//365,	
			};
			
		public:
			
			// get the number of days elapsed in this year in function of month
			// @p_month = the number of the month starting by january = 0
			// @p_year = the year ex 2020
			static int16_t getDaysElapsedFromMonths(uint8_t p_month, uint16_t p_year)
			{
				if (p_year > 2099 | p_year < 2020 | p_month > 11)
					return -1;
				if (p_month < 2) // january of february, the same for leap or non leap year
					return daysFromMonthsElapsed[p_month];
					
				auto currentYear = days2020to2100[p_year - 2020];
				if (currentYear.isLeap)
					return daysFromMonthsElapsed[p_month] + 1;	
				else
					return daysFromMonthsElapsed[p_month];
			}
			
			// get the year index in days table 
			// return -1 if out of table, index otherwise
			static int16_t getYearFromDaysSinceEpoch(uint32_t days)
			{
				if (days < days2020to2100[0].daysSinceEpoch)
					return -1;	// date is too early for system
				uint8_t index = 0;
				while (days  >= days2020to2100[index].daysSinceEpoch)
				{
					if (index >= k_tableSize)
						return -1; // date is too late for system
					index++;
				}
				return index+2019;
			}
			
			// return the number of days elapsed since Epoch (1 january 1970)
			static int32_t getDaysSinceEpoch(uint16_t p_year)
			{
				if (p_year > 2099 | p_year < 2020)
					return -1;
				return days2020to2100[p_year - 2020].daysSinceEpoch;
			}
			
			// get month from elapsed days from beginning of year and year
			//@param p_year = number of year ex 2020
			//@param p_daysFromBeginningOfTheYear : the number of days since the 1st january of this year
			// return -1 if error, number of month if success (starting by january as 0)
			static int8_t getMonthFromDaysCount(uint32_t p_daysFromBeginningOfTheYear, uint16_t p_year)
			{
				if (p_year > 2099 | p_year < 2020 )
					return -1;
				uint8_t index = 0;
				// if is a leap year, reduce of one day for compute
				if((p_daysFromBeginningOfTheYear > daysFromMonthsElapsed[1]) && (days2020to2100[p_year-2020].isLeap == true))
				{
					p_daysFromBeginningOfTheYear--;
					index = 1;
				}
				
				if (p_daysFromBeginningOfTheYear > 365)
					return -1;

				//search for the first month with superior number of days
				while(daysFromMonthsElapsed[index] < p_daysFromBeginningOfTheYear)
					index++;
				return index-1; // return the month before
			}
			
			// return the week of day from the day
			static WeekDay getWeekDayFromDaysSinceEpoch(uint32_t days)
			{
				switch (days % 7)
				{
				case 0: // same day as 1 january 1970 => thursday
					return volund::utils::Time::WeekDay::thursday;
					break;
				case 1:
					return volund::utils::Time::WeekDay::friday;
					break;
				case 2:
					return volund::utils::Time::WeekDay::saturday;
					break;
				case 3:
					return volund::utils::Time::WeekDay::sunday;
					break;
				case 4:
					return volund::utils::Time::WeekDay::monday;
					break;
				case 5:
					return volund::utils::Time::WeekDay::tuesday;
					break;
				case 6:
					return volund::utils::Time::WeekDay::wednesday;
					break;
				default: // error, impossible
					return volund::utils::Time::WeekDay::monday;
				}
			}
	};
	} // namespace utils
} // namespace volund
