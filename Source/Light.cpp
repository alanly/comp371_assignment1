#include "Light.h"


using namespace std;


Light::Light(void)
{
	lColor = glm::vec3(1.f, 1.f, 1.f);
	lCoefficients = glm::vec3(0.f, 0.f, 0.02f);
}


Light::~Light(void)
{
}

void Light::Load(ci_istringstream& iss)
{
	ci_string line;

	// Parse model line by line
	while(std::getline(iss, line))
	{
		// Splitting line into tokens
		ci_istringstream strstr(line);
		istream_iterator<ci_string, char, ci_char_traits> it(strstr);
		istream_iterator<ci_string, char, ci_char_traits> end;
		vector<ci_string> token(it, end);

		if (ParseLine(token) == false)
		{
			fprintf(stderr, "Error loading scene file... token:  %s!", token[0]);
			getchar();
			exit(-1);
		}
	}
}

bool Light::ParseLine(const std::vector<ci_string> &token)
{
	if (token.empty() == false)
	{
		if (token[0].empty() == false && token[0][0] == '#')
		{
			return true;
		}

		if (token[0] == "name")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");

			lName = token[2];
		}
		else if (token[0] == "position")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			SetLightPosition(
				static_cast<float>(atof(token[2].c_str())),
				static_cast<float>(atof(token[3].c_str())),
				static_cast<float>(atof(token[4].c_str()))
			);
		}
		else if (token[0] == "color")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			lColor.x = static_cast<float>(atof(token[2].c_str()));
			lColor.y = static_cast<float>(atof(token[3].c_str()));
			lColor.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "coefficients")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			lCoefficients.x = static_cast<float>(atof(token[2].c_str()));
			lCoefficients.y = static_cast<float>(atof(token[3].c_str()));
			lCoefficients.z = static_cast<float>(atof(token[4].c_str()));
		}
		else
		{
			return false;
		}
	}

	return true;
}