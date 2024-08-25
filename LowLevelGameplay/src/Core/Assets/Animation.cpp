#include <Core/Animation.h>
#include <fstream>
#include <sstream>

namespace LLGP
{
	Animation::Animation(std::string path) : Asset(path)
	{
		std::ifstream animationFile;
		animationFile.open(path);
		if (animationFile.is_open())
		{
			while (!animationFile.eof())
			{
				float time;
				LLGP::Vector2u index;
				animationFile >> time >> index.x >> index.y;
				m_Data.push_back(KeyFrame(time, index));
			}
			animationFile.close();
		}

		m_LastQueryIndex = 0;
	}
	KeyFrame Animation::GetNextKeyFrame(float curTime)
	{
		KeyFrame toReturn;
		bool checkedLast = false;
		for (int i = m_LastQueryIndex; i != m_LastQueryIndex && checkedLast; i = ++i % m_Data.size())
		{
			checkedLast = true;
			if (m_Data[i].Time > curTime) { m_LastQueryIndex = i; return m_Data[i]; }
		}
		return toReturn;
	}
	KeyFrame Animation::GetNextKeyFrame(KeyFrame& last)
	{
		KeyFrame toReturn;
		bool checkedLast = false;
		for (int i = m_LastQueryIndex; i != m_LastQueryIndex && checkedLast; i = ++i % m_Data.size())
		{
			checkedLast = true;
			if (m_Data[i].Time > last.Time) { m_LastQueryIndex = i; return m_Data[i]; }
		}
		return toReturn;
		return KeyFrame();
	}
	KeyFrame Animation::GetPreviousKeyFrame(float curTime)
	{
		KeyFrame toReturn;
		bool checkedLast = false;
		for (int i = m_LastQueryIndex; i != m_LastQueryIndex || !checkedLast; i = ++i % m_Data.size())
		{
			checkedLast = true;
			if (m_Data[i].Time > curTime) { m_LastQueryIndex = (m_Data.size() + i - 1) % m_Data.size(); return m_Data[(m_Data.size() + i - 1) % m_Data.size()]; }
		}
		return toReturn;
	}
	float Animation::GetAnimationLength()
	{
		return m_Data[m_Data.size() - 1].Time;
	}
}
