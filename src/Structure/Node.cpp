#include <cassert>
#include <Structure/Node.h>
#include <Devices/Amplifiers/PreAmplifier.h>
#include <Devices/Amplifiers/BoosterAmplifier.h>
#include <Devices/SSS.h>
#include <Devices/Splitter.h>

Node::Node(int ID, Node_Type T, Node_Architecure A,
		   int NumRegenerators) : ID(ID), Type(T) , Architecture(A) ,
	NumRegenerators(NumRegenerators) {

	create_Devices();

}

bool Node::operator ==(Node *N) {
	if (ID == N->ID) {
		return true;
	}

	return false;
}

void Node::insert_Link(Node *N, std::shared_ptr<Link> Link) {
	bool LinkExists = false;

	for (auto it = Neighbours.begin(); it != Neighbours.end(); ++it) {
		if (*it == N) {
			LinkExists = true;
			break;
		}
	}

	if (!LinkExists) {
		Neighbours.push_back(N);
		Links.push_back(Link);

		if (Architecture == BroadcastAndSelect) {
			assert((*Devices.front()).T == Device::SplitterDevice);
			static_cast<Splitter &>(*Devices.front()).set_NumPorts(Links.size());
		}
	}
}

Node::Node_Architecure Node::get_NodeArch() {
	return Architecture;
}

void Node::create_Devices() {
	//Switching element - entrance
	if (Architecture == BroadcastAndSelect) {
		Devices.push_back(std::unique_ptr<Device>(new Splitter(Links.size())));
	} else {
		Devices.push_back(std::unique_ptr<Device>(new SSS()));
	}

	//Switching element - exit
	Devices.push_back(std::unique_ptr<Device>(new SSS()));

	//Booster AmplifierD
	Devices.push_back(std::unique_ptr<Device>(new BoosterAmplifier()));
}

Signal Node::bypass(Signal S) {
	for (auto it = Devices.begin(); it != Devices.end(); ++it) {
		S *= (*it)->get_Gain();
		S *= (*it)->get_Loss();
		S += (*it)->get_Noise();
	}

	return S;
}

Signal Node::add(Signal S) {
	for (auto it = Devices.begin(); it != Devices.end(); ++it) {
		S *= (*it)->get_Gain();
		S *= (*it)->get_Loss();
		S += (*it)->get_Noise();

		if ((*it)->T == Device::SplitterDevice || (*it)->T == Device::SSSDevice) {
			break;
		}
	}

	return S;
}

Signal Node::drop(Signal S) {
	bool foundExit = false;

	for (auto it = Devices.begin(); it != Devices.end(); ++it) {
		if (!foundExit && (*it)->T != Device::SplitterDevice &&
				(*it)->T != Device::SSSDevice) {
			foundExit = true;
			continue;
		}

		S *= (*it)->get_Gain();
		S *= (*it)->get_Loss();
		S += (*it)->get_Noise();
	}

	return S;
}
