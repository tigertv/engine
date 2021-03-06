/**
 * @file
 */

#pragma once

#include "Task.h"
#include "backend/entity/Npc.h"

namespace backend {

AI_TASK(SetPointOfInterest) {
	backend::Npc& npc = chr.getNpc();
	npc.setPointOfInterest();
	return ai::TreeNodeStatus::FINISHED;
}

}

