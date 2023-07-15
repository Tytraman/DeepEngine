#ifndef __DEEP_ENGINE_PANEL_HPP__
#define __DEEP_ENGINE_PANEL_HPP__

#include <DE/def.h>
#include <DE/renderer.hpp>

#include <stdint.h>
#include <vector>

namespace de {

	class Drawable;

	namespace PanelType {

		enum e : uint32_t {
			Window,
			Drawable,
			Custom
		};

	}

	/// @class Panel
	/// @brief Classe mère de la gestion des groupements d'éléments.
	///
	/// Les panels servent à grouper plusieurs éléments dans un seul conteneur.
	class DE_API Panel {

		public:
			typedef void (*PanelEnumProc)(Panel *panel, void *args);

		protected:
			Panel *_parent;					///< Pointeur vers le \ref Panel parent, \c nullptr si aucun parent.
			PanelType::e _type;				///< Type de panel représenté.
			size _size;						///< La taille du panel en pixels.
			std::vector<Panel *> *_panels;	///< Liste des panels enfants.

		public:
			Panel() = delete;

			/// @brief			Constructeur du panel.
			/// @param parent	Pointeur vers le panel parent ou \c nullptr si aucun parent.
			/// @param type		Type de panel créé.
			/// @param size		Taille en pixels du panel.
            Panel(Panel *parent, PanelType::e type, const size &size);

			/// @brief Vide la mémoire interne utilisée par le panel.
			~Panel();

			void enumeratePanels(PanelEnumProc proc, void *args);

			/// @brief Détruit le panel sans détruire ses enfants.
			virtual void destroy();

			/// @brief			Ajoute un panel enfant.
			/// @param panel	Pointer du panel enfant à ajouter.
			/// @remark			Il est à absolument éviter de rendre un panel enfant à plusieurs panels.
			void addPanel(Panel *panel);

			/// @brief			Défini la taille du panel.
			/// @param size		La taille à définir.
			void setSize(const size &size);

			/// @brief	Récupère le parent du panel.
			/// @return Pointeur vers le panel parent ou \c nullptr si aucun parent.
			Panel *getParent();

			/// @brief	Récupère le type du panel.
			/// @return \ref PanelType représentant le type du panel.
			PanelType::e getType() const;

			/// @brief	Récupère la taille du panel.
			/// @return La taille en pixels.
			size getSize() const;

			/// @brief			Récupère un panel enfant.
			/// @param index	Le numéro d'index du panel enfant à récupérer.
			/// @return			Pointeur vers le panel enfant.
			Panel *getPanel(size_t index);

	};

	/*
	==============
	Panel::destroy
	==============
	*/
	inline void Panel::destroy()
	{
		delete _panels;
		_panels = nullptr;
	}

	/*
	==============
	Panel::setSize
	==============
	*/
	inline void Panel::setSize(const size &size)
	{
		_size = size;
	}

	/*
	================
	Panel::getParent
	================
	*/
	inline Panel *Panel::getParent()
	{
		return _parent;
	}

	/*
	==============
	Panel::getType
	==============
	*/
	inline PanelType::e Panel::getType() const
	{
		return _type;
	}

	/*
	==============
	Panel::getSize
	==============
	*/
	inline size Panel::getSize() const
	{
		return _size;
	}

	class DE_API DrawablePanel : public Panel {

		protected:
			Renderer _renderer;
			fmat2x2 _baseVectors;
			std::vector<Drawable *> *_drawables;
			colora _backColor;

		public:
			DrawablePanel() = delete;
			DrawablePanel(Panel *parent, const size &size, bool showXAxis = false, bool showYAxis = false, const fvec2 &baseIVector = fvec2(1.0f, 0.0f), const fvec2 &baseJVector = fvec2(0.0f, 1.0f));
			~DrawablePanel();

			void destroy() override;

			static ErrorStatus create(DrawablePanel &dest);

			void clear();
			void render();

			void addDrawable(Drawable *drawable);
			Drawable *getDrawable(size_t index);

			Renderer *getRenderer();
			fvec2 getBaseIVector() const;
			fvec2 getBaseJVector() const;
			colora getBackColor() const;

			void setRenderer(Renderer *renderer);
			void setBaseIVector(const fvec2 &baseVector);
			void setBaseJVector(const fvec2 &baseVector);

	};

	/*
	======================
	DrawablePanel::destroy
	======================
	*/
	inline void de::DrawablePanel::destroy()
	{
		_renderer.destroy();
		// Attention car si le panel a des enfants, en supprimant le panel, les enfants ne seront pas
		// supprimés, c'est donc à celui qui détruit le premier panel de s'assurer de d'abord détruire les enfants.
		// TODO: La classe Window doit le faire.
		Panel::destroy();
	}

	/*
	====================
	DrawablePanel::clear
	====================
	*/
	inline void DrawablePanel::clear()
	{
		_renderer.setColor(_backColor);
		_renderer.clear();
	}

	/*
	==========================
	DrawablePanel::getRenderer
	==========================
	*/
	inline Renderer *DrawablePanel::getRenderer()
	{
		return &_renderer;
	}

	/*
	===========================
	DrawablePanel::getBackColor
	==========================
	*/
	inline colora DrawablePanel::getBackColor() const
	{
		return _backColor;
	}

	/*
	==========================
	DrawablePanel::setRenderer
	==========================
	*/
	inline void DrawablePanel::setRenderer(Renderer *renderer)
	{
		_renderer = *renderer;
	}

	/*
	=============================
	DrawablePanel::getBaseIVector
	=============================
	*/
	inline fvec2 DrawablePanel::getBaseIVector() const
	{
		return { _baseVectors.x1, _baseVectors.y1 };
	}

	/*
	=============================
	DrawablePanel::getBaseJVector
	=============================
	*/
	inline fvec2 DrawablePanel::getBaseJVector() const
	{
		return { _baseVectors.x2, _baseVectors.y2 };
	}

	/*
	=============================
	DrawablePanel::setBaseIVector
	=============================
	*/
	inline void DrawablePanel::setBaseIVector(const fvec2 &baseVector)
	{
		_baseVectors.x1 = baseVector.x;
		_baseVectors.y1 = baseVector.y;
	}

	/*
	=============================
	DrawablePanel::setBaseJVector
	=============================
	*/
	inline void DrawablePanel::setBaseJVector(const fvec2 &baseVector)
	{
		_baseVectors.x2 = baseVector.x;
		_baseVectors.y2 = baseVector.y;
	}


}

#endif