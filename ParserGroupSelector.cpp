#include "ParserGroupSelector.h"
#include "WaveObject.h"

ParserGroupSelector::ParserGroupSelector(WaveObject* object)
{
	this->object = object;
}

void ParserGroupSelector::parse(vector<string>& line)
{
	string groupName = line.at(1);

	Group* group = new Group(groupName);

	object->groups.push_back(group);
	object->currentGroup = group;
}

