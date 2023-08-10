#ifndef __DEEP_ENGINE_VEC_HPP__
#define __DEEP_ENGINE_VEC_HPP__

#include <DE/def.h>

#include <stdint.h>

namespace de {

	class Renderer;
	struct fmat2x2;
	struct fmat3x3;

	namespace Vec2Direction {
		enum e : uint8_t {
			Up,
			Left,
			Down,
			Right,
			None
		};
	}

	/// @struct vec2
	/// @brief  Vecteur à 2 dimensions avec des composantes de type \c int32_t.
	struct DE_API vec2 {
		int32_t x;    ///< Composante X du vecteur.
		int32_t y;    ///< Composante Y du vecteur.

		/// @brief   Constructeur de vecteurs à 2 dimensions de type \c int32_t.
		/// @param x Valeur de la composante X.
		/// @param y Valeur de la composante Y.
		vec2(int32_t x = 0, int32_t y = 0);

		/// @brief          Trace le vecteur à partir de l'origine.
		/// @param renderer Le renderer qui s'occupe de dessiner la ligne.
		void draw(Renderer &renderer) const;
	};

	/*
	==========
	vec2::vec2
	==========
	*/
	inline vec2::vec2(int32_t _x, int32_t _y)
		: x(_x), y(_y)
	{ }

	/// @struct fvec2
	/// @brief	Vecteur à 2 dimensions avec des composantes de type flottant.
	struct DE_API fvec2 {
		float x;    ///< Composante X du vecteur.
		float y;    ///< Composante Y du vecteur.

		/// @brief   Constructeur de vecteurs à 2 dimensions de type flottant.
		/// @param x Valeur de la composante X.
		/// @param y Valeur de la composante Y.
		fvec2(float x = 0.0f, float y = 0.0f);

		/// @brief          Trace le vecteur à partir de l'origine.
		/// @param renderer Le renderer qui s'occupe de dessiner la ligne.
		void draw(Renderer &renderer) const;

		/// @brief     Surcharge qui retourne le résultat de cette fonction : \ref add(const fvec2 &v1, const fvec2 &v2).
		/// @param vec Le vecteur à additionner avec le vecteur courant.
		/// @return    Le vecteur résultant du calcul d'addition entre 2 vecteurs.
		fvec2 operator+(const fvec2 &vec) const;

		/// @brief     Surcharge qui retourne le résultat de cette fonction : \ref add(const fvec2 &v1, const vec2 &v2).
		/// @param vec Le vecteur à additionner avec le vecteur courant.
		/// @return    Le vecteur résultant du calcul d'addition entre 2 vecteurs.
		fvec2 operator+(const vec2 &vec) const;

		/// @brief            Surcharge qui retourne le résultat de cette fonction : \ref add(const fvec2 &v, float valueToAdd).
		/// @param valueToAdd La valeur à ajouter au vecteur courant.
		/// @return           Le vecteur résultant du calcul d'addition entre un vecteur et une valeur constante.
		fvec2 operator+(float valueToAdd) const;

		/// @brief   Surcharge qui retourne le résultat de cette fonction : \ref sub(const fvec2 &v1, const fvec2 &v2).
		/// @param v Le vecteur à soustraire du vecteur courant.
		/// @return  Le vecteur résultant du calcul de soustraction entre 2 vecteurs.
		fvec2 operator-(const fvec2 &v) const;

		/// @brief            Surcharge qui retourne le résultat de cette fonction : \ref sub(const fvec2 &v, float valueToSub).
		/// @param valueToSub La valeur à retirer du vecteur courant.
		/// @return           Le vecteur résultant du calcul de soustraction entre un vecteur et une valeur constante.
		fvec2 operator-(float valueToSub) const;

		/// @brief        Surcharge qui retourne le résultat de cette fonction : \ref scale(const fvec2 &v, float scalar).
		/// @param scalar La valeur scalaire à multiplier au vecteur courant.
		/// @return       Le vecteur résultant de la multiplication scalaire entre un vecteur et une constante.
		fvec2 operator*(float scalar) const;

		/// @brief     Surcharge qui retourne le résultat de cette fonction : \ref mul(const fvec2 &v, const fmat2x2 &mat).
		/// @param mat La matrice à multiplier au vecteur courant.
		/// @return    Le vecteur résultant de la multiplication entre un vecteur et une matrice.
		fvec2 operator*(const fmat2x2 &mat) const;

		/// @brief       Surcharge qui effectue cette fonction : \ref sub(const fvec2 &v, float valueToSub) sur le vecteur courant.
		/// @param value La valeur à retirer du vecteur courant.
		void operator-=(float value);

		/// @brief   Surcharge qui effectue cette fonction \ref add(const fvec2 &v1, const vec2 &v2) sur le vecteur courant.
		/// @param v Le vecteur à ajouter au vecteur courant.
		void operator+=(const fvec2 &v);

		/// @brief       Surcharge qui effectue cette fonction : \ref add(const fvec2 &v, float valueToAdd) sur le vecteur courant.
		/// @param value La valeur à ajouter au vecteur courant.
		void operator+=(float value);

		/// @brief        Surcharge qui effectue cette fonction : \ref scale(const fvec2 &v, float scalar) sur le vecteur courant.
		/// @param scalar Le scalaire à multiplier sur le vecteur courant.
		void operator*=(float scalar);

		/// @brief    Additionne 2 vecteurs entre eux et retourne le résultat.
		/// @param v1 Vecteur 1.
		/// @param v2 Vecteur 2.
		/// @remark   La formule d'addition de 2 vecteurs est comme suit :<br>
		///           <img src="vec2_addition.png" align="left">
		///           <div style="clear: both"></div>
		/// @return   Le vecteur résultant de ce calcul.
		static fvec2 add(const fvec2 &v1, const fvec2 &v2);

		/// @brief    Additionne 2 vecteurs entre eux et retourne le résultat.
		/// @param v1 Vecteur 1.
		/// @param v2 Vecteur 2.
		/// @remark   Voir \ref add(const fvec2 &v1, const fvec2 &v2).
		/// @return   Le vecteur résultant de ce calcul.
		static fvec2 add(const fvec2 &v1, const vec2 &v2);

		/// @brief            Additionne un vecteur avec une constante et retourne le résultat.
		/// @param vec        Vecteur.
		/// @param valueToAdd Constante.
		/// @remark           La formule d'addition entre un vecteur et une constante est comme suit :<br>
		///                   <img src="vec2_addition_const.png" align="left">
		///                   <div style="clear: both"></div>
		/// @return           Le vecteur résultant de ce calcul.
		static fvec2 add(const fvec2 &v, float valueToAdd);

		/// @brief    Retourne le résultat de la soustraction d'un vecteur par un autre.
		/// @param v1 Vecteur 1.
		/// @param v2 Vecteur 2.
		/// @remark   La formule de soustraction de 2 vecteurs est comme suit :<br>
		///           <img src="vec2_substraction.png" align="left">
		///           <div style="clear: both"></div>
		/// @return   Le vecteur résultant de ce calcul.
		static fvec2 sub(const fvec2 &v1, const fvec2 &v2);

		/// @brief            Retourne le résultat de la soustraction d'un vecteur par une constante.
		/// @param v          Vecteur.
		/// @param valueToSub Constante.
		/// @remark           La formule de soustraction entre un vecteur et une constante est comme suit :<br>
		///                   <img src="vec2_substraction_const.png" align="left">
		///                   <div style="clear: both"></div>
		/// @return           Le vecteur résultant de ce calcul.
		static fvec2 sub(const fvec2 &v, float valueToSub);

		/// @brief        Retourne la multiplication scalaire entre un vecteur et une constante.
		/// @param v      Vecteur.
		/// @param scalar Constante.
		/// @remark       La formule de la multiplication scalaire entre un vecteur et une constante est comme suit :<br>
		///               <img src="vec2_scale_const.png" align="left">
		///               <div style="clear: both"></div>
		///               Cela permet d'agrandir / réduire la longueur d'un vecteur et / ou de changer sa direction.
		/// @return       Le vecteur résultant de ce calcul.
		static fvec2 scale(const fvec2 &v, float scalar);

		/// @brief    Retourne le résultat du produit scalaire entre 2 vecteurs.
		/// @param v1 Vecteur 1.
		/// @param v2 Vecteur 2.
		/// @remark   Les formules du produit scalaire entre 2 vecteurs sont comme suit :<br>
		///           <div>
		///             <div style="float:left;">
		///               <img src="vec2_dot_product_algebraic.png" style="display:block;margin:auto;"><br>
		///               <span>Définition algébrique, utilisée par le moteur.</span>
		///             </div>
		///             <div style="float:left;margin-left:10px;">
		///               <img src="vec2_dot_product_geometric.png" style="display:block;margin:auto;"><br>
		///               <span>Définition géométrique.</span>
		///             </div>
		///           </div>
		///           <div style="clear: both"></div>
		/// @return   Le résultat du calcul.
		static float dot(const fvec2 &v1, const fvec2 &v2);

		/// @brief   Retourne la magnitude <i>(la longueur)</i> d'un vecteur.
		/// @param v Vecteur.
		/// @remark  La formule permettant de calculer la magnitude d'un vecteur est la suivante :<br>
		///          <img src="vec2_magn.png" align="left">
		///          <div style="clear:both"></div>
		/// @return  La magnitude du vecteur.
		static float magn(const fvec2 &v);

		static fvec2 norm(const fvec2 &v);

		/// @brief    Retourne l'angle en degrés entre 2 vecteurs.
		/// @param v1 Vecteur 1.
		/// @param v2 Vecteur 2.
		/// @remark   La formule pour calculer l'angle entre 2 vecteurs est la suivante :<br>
		///           <img src="vec2_angle.png" align="left">
		///           <div style="clear:both"></div>
		/// @return   L'angle en degrés entre ces 2 vecteurs.
		static float angle(const fvec2 &v1, const fvec2 &v2);

		/// @brief     Retourne le résultat de la multiplication entre un vecteur et une matrice.
		/// @param v   Vecteur.
		/// @param mat Matrice.
		/// @remark    La formule de multiplication entre un vecteur et une matrice est la suivante :<br>
		///            <img src="vec2_mat_multiplication.png" align="left">
		///            <div style="clear:both"></div>
		/// @return    Le vecteur résultant de ce calcul.
		static fvec2 mul(const fvec2 &v, const fmat2x2 &mat);

		/// @brief   Retourne le vecteur avec une direction opposé à celui passé en paramètre.
		/// @param v Le vecteur à inverser.
		/// @return  Le vecteur inversé.
		static fvec2 inv(const fvec2 &v);

		/// @brief    Fait une interpolation linéaire entre 2 vecteurs pour récupérer la composante X à une valeur Y.
		/// @param v1 Vecteur 1.
		/// @param v2 Vecteur 2.
		/// @param y  La valeur Y à laquelle interpoler X.
		/// @return   La valeur d'interpolation X entre ces 2 vecteurs.
		static fvec2 interpolateX(const fvec2 &v1, const fvec2 &v2, float y);

		/// @brief    Fait une interpolation linéaire entre 2 vecteurs pour récupérer la composante Y à une valeur X.
		/// @param v1 Vecteur 1.
		/// @param v2 Vecteur 2.
		/// @param x  La valeur X à laquelle interpoler Y.
		/// @return   La valeur d'interpolation Y entre ces 2 vecteurs.
		static fvec2 interpolateY(const fvec2 &v1, const fvec2 &v2, float x);
	};

	/// @struct fvec3
	/// @brief	Vecteur à 3 dimensions avec des composantes de type flottant.
	struct DE_API fvec3 {
		float x;    ///< Composante X du vecteur.
		float y;    ///< Composante Y du vecteur.
		float z;    ///< Composante Z du vecteur.

		/// @brief   Constructeur de vecteurs à 3 dimensions de type flottant.
		/// @param x Valeur de la composante X.
		/// @param y Valeur de la composante Y.
		/// @param z Valeur de la composante Z.
		fvec3(float x = 0.0f, float y = 0.0f, float z = 1.0f);

		/// @brief   Surcharge qui retourne le résultat de cette fonction : \ref add(const fvec3 &v1, const fvec3 &v2).
		/// @param v Le vecteur à additionner avec le vecteur courant.
		/// @return  Le vecteur résultant du calcul d'addition entre 2 vecteurs.
		fvec3 operator+(const fvec3 &v) const;

		/// @brief            Surcharge qui retourne le résultat de cette fonction : \ref add(const fvec3 &v, float valueToAdd).
		/// @param valueToAdd La valeur à ajouter au vecteur courant.
		/// @return           Le vecteur résultant du calcul d'addition entre un vecteur et une valeur constante.
		fvec3 operator+(float valueToAdd) const;

		/// @brief   Surcharge qui retourne le résultat de cette fonction : \ref sub(const fvec3 &v1, const fvec3 &v2).
		/// @param v Le vecteur à soustraire du vecteur courant.
		/// @return  Le vecteur résultant du calcul de soustraction entre 2 vecteurs.
		fvec3 operator-(const fvec3 &v) const;

		/// @brief            Surcharge qui retourne le résultat de cette fonction : \ref sub(const fvec3 &v, float valueToSub).
		/// @param valueToSub La valeur à retirer du vecteur courant.
		/// @return           Le vecteur résultant du calcul de soustraction entre un vecteur et une valeur constante.
		fvec3 operator-(float valueToSub) const;

		/// @brief        Surcharge qui retourne le résultat de cette fonction : \ref scale(const fvec3 &v, float scalar).
		/// @param scalar La valeur scalaire à multiplier au vecteur courant.
		/// @return       Le vecteur résultant de la multiplication scalaire entre un vecteur et une constante.
		fvec3 operator*(float scalar) const;

		/// @brief     Surcharge qui retourne le résultat de cette fonction : \ref mul(const fvec3 &v, const fmat3x3 &mat).
		/// @param mat La matrice à multiplier au vecteur courant.
		/// @return    Le vecteur résultant de la multiplication entre un vecteur et une matrice.
		fvec3 operator*(const fmat3x3 &mat) const;

		/// @brief        Surcharge qui effectue cette fonction : \ref scale(const fvec3 &v, float scalar) sur le vecteur courant.
		/// @param scalar Le scalaire à multiplier sur le vecteur courant.
		void operator*=(float scalar);

		/// @brief    Retourne le résultat de l'addition entre 2 vecteurs.
		/// @param v1 Vecteur 1.
		/// @param v2 Vecteur 2.
		/// @remark   La formule d'addition de 2 vecteurs est la suivante :<br>
		///           <img src="vec3_addition.png" align="left">
		///           <div style="clear:both"></div>
		/// @return   Le vecteur résultant de ce calcul.
		static fvec3 add(const fvec3 &v1, const fvec3 &v2);

		/// @brief            Retourne le résultat de l'addition entre un vecteur et une constante.
		/// @param v          Vecteur.
		/// @param valueToAdd Constante.
		/// @remark           La formule d'addition d'un vecteur avec une constante est la suivante :<br>
		///                   <img src="vec3_addition_const.png" align="left">
		///                   <div style="clear:both"></div>
		/// @return           Le vecteur résultant de ce calcul.
		static fvec3 add(const fvec3 &v, float valueToAdd);

		/// @brief    Retourne le résultat de la soustraction d'un vecteur par un autre.
		/// @param v1 Vecteur 1.
		/// @param v2 Vecteur 2.
		/// @remark   La formule de soustraction d'un vecteur par un autre est la suivante :<br>
		///           <img src="vec3_substraction.png" align="left">
		///           <div style="clear:both"></div>
		/// @return   Le vecteur résultant de ce calcul.
		static fvec3 sub(const fvec3 &v1, const fvec3 &v2);

		/// @brief            Retourne le résultat de la soustraction entre un vecteur et une constante.
		/// @param v          Vecteur.
		/// @param valueToSub Constante.
		/// @remark           La formule de soustraction d'un vecteur par une constante est la suivante :<br>
		///                   <img src="vec3_substraction_const.png" align="left">
		///                   <div style="clear:both"></div>
		/// @return           Le vecteur résultant de ce calcul.
		static fvec3 sub(const fvec3 &v, float valueToSub);

		/// @brief        Retourne la multiplication scalaire entre un vecteur et une constante.
		/// @param v      Vecteur.
		/// @param scalar Scalaire.
		/// @remark       La formule de la multiplication scalaire d'un vecteur par une constante est la suivante :<br>
		///               <img src="vec3_scale_const.png" align="left">
		///               <div style="clear:both"></div>
		///               Cela permet d'agrandir / réduire la longueur d'un vecteur et / ou de changer sa direction.
		/// @return       Le vecteur résultant de ce calcul.
		static fvec3 scale(const fvec3 &v, float scalar);

		/// @brief    Retourne le résultat du produit scalaire entre 2 vecteurs.
		/// @param v1 Vecteur 1.
		/// @param v2 Vecteur 2.
		/// @remark   Les formules du produit scalaire entre 2 vecteurs sont comme suit :<br>
		///           <div>
		///             <div style="float:left;">
		///               <img src="vec3_dot_product_algebraic.png" style="display:block;margin:auto;"><br>
		///               <span>Définition algébrique, utilisée par le moteur.</span>
		///             </div>
		///             <div style="float:left;margin-left:10px;">
		///               <img src="vec2_dot_product_geometric.png" style="display:block;margin:auto;"><br>
		///               <span>Définition géométrique.</span>
		///             </div>
		///           </div>
		///           <div style="clear: both"></div>
		/// @return   Le résultat du calcul.
		static float dot(const fvec3 &v1, const fvec3 &v2);

		/// @brief   Retourne la magnitude <i>(la longueur)</i> d'un vecteur.
		/// @param v Vecteur.
		/// @remark  La formule permettant de calculer la magnitude d'un vecteur est la suivante :<br>
		///          <img src="vec3_magn.png" align="left">
		///          <div style="clear:both"></div>
		/// @return  La magnitude du vecteur.
		static float magn(const fvec3 &v);

		/// @brief    Retourne l'angle en degrés entre 2 vecteurs.
		/// @param v1 Vecteur 1.
		/// @param v2 Vecteur 2.
		/// @remark   La formule pour calculer l'angle entre 2 vecteurs est la suivante :<br>
		///           <img src="vec2_angle.png" align="left">
		///           <div style="clear:both"></div>
		/// @return   L'angle en degrés entre ces 2 vecteurs.
		static float angle(const fvec3 &v1, const fvec3 &v2);

		/// @brief     Retourne le résultat de la multiplication entre un vecteur et une matrice.
		/// @param v   Vecteur.
		/// @param mat Matrice.
		/// @remark    La formule de multiplication entre un vecteur et une matrice est la suivante :<br>
		///            <img src="vec3_mat_multiplication.png" align="left">
		///            <div style="clear:both"></div>
		/// @return    Le vecteur résultant de ce calcul.
		static fvec3 mul(const fvec3 &v, const fmat3x3 &mat);

		/// @brief   Retourne le vecteur avec une direction opposé à celui passé en paramètre.
		/// @param v Le vecteur à inverser.
		/// @return  Le vecteur inversé.
		static fvec3 inv(const fvec3 &v);
	};

}

#endif