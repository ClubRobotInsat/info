/*
 * Generated by PetriLab - https://sigilence-technologies.com/petrilab
 * Version 3.0.0-beta1
 */

#ifndef PETRI_GENERATED_GoberCube_H
#define PETRI_GENERATED_GoberCube_H

#include <memory>
#include <petrilab/Cpp/MemberPetriDynamicLib.h>

namespace Petri {
	namespace Generated {
		namespace GoberCube {
			enum Petri_Param_Enum : std::uint_fast32_t { Petri_Param_Enum_Vcommande = 0 };

			std::unique_ptr<::Petri::PetriNet> createPetriNet();
			std::unique_ptr<::Petri::PetriDebug> createDebugPetriNet();

			inline std::unique_ptr<::Petri::PetriDynamicLib> createLib(std::string const& customPath = "") {
				return std::make_unique<::Petri::MemberPetriDynamicLib>(false, "GoberCube", 12346, customPath);
			}
		}
	}
}

#endif
