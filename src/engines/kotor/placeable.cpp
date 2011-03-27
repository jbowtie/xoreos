/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010-2011 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file engines/kotor/placeable.cpp
 *  A placeable.
 */

#include "engines/kotor/placeable.h"

#include "common/util.h"
#include "common/error.h"
#include "common/ustring.h"

#include "engines/aurora/util.h"
#include "engines/aurora/model.h"

#include "aurora/2dafile.h"
#include "aurora/2dareg.h"
#include "aurora/gfffile.h"

#include "graphics/aurora/model.h"

static const uint32 kUTPID = MKID_BE('UTP ');

namespace Engines {

namespace KotOR {

Placeable::Placeable() : _appearance(0xFFFFFFFF), _model(0) {
}

Placeable::~Placeable() {
	delete _model;
}

void Placeable::load(const Common::UString &name) {
	Aurora::GFFFile utp;
	loadGFF(utp, name, Aurora::kFileTypeUTP, kUTPID);

	const Aurora::GFFStruct &top = utp.getTopLevel();
	_appearance = top.getUint("Appearance", 0xFFFFFFFF);

	_tag = top.getString("Tag");

	if (_appearance == 0xFFFFFFFF)
		throw Common::Exception("No appearance");

	loadModel();
}

void Placeable::show() {
	if (_model)
		_model->show();
}

void Placeable::hide() {
	if (_model)
		_model->hide();
}

void Placeable::changedPosition() {
	if (_model)
		_model->setPosition(_position[0], _position[1], _position[2]);
}

void Placeable::changedOrientation() {
	if (_model)
		_model->setRotation(_orientation[0], _orientation[2], -_orientation[1]);
}

void Placeable::loadModel() {
	const Aurora::TwoDAFile &twoda = TwoDAReg.get("placeables");

	_model = loadModelObject(twoda.getCellString(_appearance, "modelname"));
	if (_model) {
		_model->setTag(_tag);
		_model->setClickable(true);
	}

}

} // End of namespace KotOR

} // End of namespace Engines
