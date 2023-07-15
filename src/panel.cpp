#include <DE/panel.hpp>
#include <DE/drawable.hpp>

struct panel_snapshot_s {
	de::Panel *panel;
	size_t current;

	panel_snapshot_s(de::Panel * panel, size_t current)
	{
		this->panel = panel;
		this->current = current;
	}
};

/*
============
Panel::Panel
============
*/
de::Panel::Panel(Panel *parent, PanelType::e type, const size &size)
	: _parent(parent),
	_type(type),
	_size(size),
	_panels(nullptr)
{
}

/*
=============
Panel::~Panel
=============
*/
de::Panel::~Panel()
{
	if(_panels != nullptr)
		delete _panels;
}

/*
======================
Panel::enumaratePanels
======================
*/
void de::Panel::enumeratePanels(PanelEnumProc proc, void *args)
{
	std::vector<panel_snapshot_s> snapshots; // Liste des snapshots
	size_t index = 0;

	snapshots.emplace_back(this, 0);

	while(true) {
		// Si le conteneur n'a pas d'enfants,
		// ou qu'on a parcouru tous ses enfants
		if(snapshots[index].panel->_panels == nullptr || snapshots[index].current >= snapshots[index].panel->_panels->size()) {
			// Appelle la procédure d'énumération
			if(proc != nullptr)
				proc(snapshots[index].panel, args);
			snapshots.pop_back();

			if(snapshots.size() == 0)
				break; // Quitte la boucle

			index--;
		} else {
			snapshots.emplace_back((*snapshots[index].panel->_panels)[snapshots[index].current], 0);
			snapshots[index].current++;

			index++;
		}
	}
}

/*
===============
Panel::addPanel
===============
*/
void de::Panel::addPanel(Panel *panel)
{
	_panels->push_back(panel);
}

/*
===============
Panel::getPanel
===============
*/
de::Panel *de::Panel::getPanel(size_t index)
{
	return (*_panels)[index];
}

/*
============================
DrawablePanel::DrawablePanel
============================
*/
de::DrawablePanel::DrawablePanel(Panel *parent, const size &size, bool showXAxis, bool showYAxis, const fvec2 &baseIVector, const fvec2 &baseJVector)
	: Panel(parent, PanelType::Drawable, size),
	_baseVectors(baseIVector.x, baseIVector.y, baseJVector.x, baseJVector.y),
	_drawables(new std::vector<Drawable *>())
{
}

/*
=============================
DrawablePanel::~DrawablePanel
=============================
*/
de::DrawablePanel::~DrawablePanel()
{
	delete _drawables;
}

/*
=====================
DrawablePanel::create
=====================
*/
de::ErrorStatus de::DrawablePanel::create(DrawablePanel &dest)
{
	// Vérifie si le panel est une fenêtre,
	// si non, parcourt tous les panels parents jusqu'à trouver une fenêtre.
	Panel *pan = &dest;
	Panel *parent;
	while(pan->getType() != PanelType::Window) {
		parent = pan->getParent();

		// S'il n'y a plus de parents et qu'aucune fenêtre n'a été trouvée, retourne une erreur.
		if(parent == nullptr)
			return ErrorStatus::NoWindowFound;
		pan = parent;
	}

	return Renderer::create(dest._renderer, *((Window *) pan));
}

/*
=====================
DrawablePanel::render
=====================
*/
void de::DrawablePanel::render()
{
	size_t i;

	//// Fait le rendu des axes
	//_renderer.setColor(colora(255, 255, 255, 255));
	//if(_showXAxis) {
	//	
	//	fvec2 pos1 = fvec2(0.0f, _size.height / 2.0f) * _baseVectors;					// Position 1 de base
	//	fvec2 pos2 = fvec2((float) _size.width, _size.height / 2.0f) * _baseVectors;	// Position 2 de base

	//	fvec2 finalPos1;								// Position 1 finale

	//	fvec2 vDiff = pos2 - pos1;						// Différence entre les 2 positions de bases

	//	int8_t reverso = vDiff.y >= 0 ? 1 : -1;			// Inverse la direction du vecteur si vDiff.x == 0

	//	fvec2 vAdd;										// Calcule la valeur Y pour la nouvelle coordonnée d'ajout

	//	float targetY;
	//	float yDiff;

	//	fvec2 vTotal;

	//	if(vDiff.x == 0) {
	//		vAdd = fvec2(0, _size.height * reverso);
	//		yDiff = vAdd.y - pos2.y;

	//		targetY = _size.height * reverso;
	//		vTotal = fvec2(pos1.x, targetY);

	//		finalPos1 = fvec2(pos1.x, _size.height);
	//	}else {
	//		vAdd = fvec2(vDiff.x + pos1.x, vDiff.y * (vDiff.x + pos1.x) / vDiff.x);
	//		yDiff = vAdd.y - pos2.y;

	//		targetY = vDiff.y * _size.width / vDiff.x;
	//		vTotal = fvec2(_size.width, targetY);

	//		finalPos1 = fvec2(0, -yDiff);
	//	}

	//	fvec2 finalPos2(vTotal.x, vTotal.y - yDiff);	// Position 2 finale

	//	fvec2 linearInterpolation1 = fvec2::interpolateX(finalPos1, finalPos2, 0);
	//	bool enabled = true;

	//	if(finalPos1.x < 0 || finalPos1.y < 0) {
	//		if(linearInterpolation1.x < 0)
	//			enabled = false;
	//		else
	//			finalPos1 = linearInterpolation1;
	//	}
	//		
	//	if(enabled) {
	//		fvec2 pos2x;
	//		fvec2 pos2xx;

	//		if(finalPos1.y > finalPos2.y)
	//			pos2x = linearInterpolation1;
	//		else
	//			pos2x = finalPos2;

	//		if(pos2x.x > _size.width)
	//			pos2xx = finalPos2;
	//		else
	//			pos2xx = pos2x;

	//		_renderer.drawLine((int) finalPos1.x, (int) finalPos1.y,  (int) pos2xx.x, (int) pos2xx.y);
	//	}
	//}
	//	
	//if(_showYAxis) {
	//	fvec2 pos1 = fvec2(_size.width / 2.0f, 0.0f) * _baseVectors;
	//	fvec2 pos2 = fvec2(_size.width / 2.0f, _size.height) * _baseVectors;
	//	//_renderer.drawLine((int) pos1.x, (int) pos1.y,  (int) pos2.x, (int) pos2.y);
	//}
	
	// Dessine tous les drawables stockés dans le panel
	for(i = 0; i < _drawables->size(); ++i)
		if((*_drawables)[i]->_visible)
			(*_drawables)[i]->draw(_baseVectors);

	// Swap les buffers du renderer pour mettre à jour le rendu
	_renderer.swapBuffers();
}

/*
==========================
DrawablePanel::addDrawable
==========================
*/
void de::DrawablePanel::addDrawable(Drawable *drawable)
{
	_drawables->push_back(drawable);
}

/*
==========================
DrawablePanel::getDrawable
==========================
*/
de::Drawable *de::DrawablePanel::getDrawable(size_t index)
{
	return (*_drawables)[index];
}