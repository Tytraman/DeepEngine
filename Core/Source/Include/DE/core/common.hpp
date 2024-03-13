#ifndef __DEEP_ENGINE_COMMON_HPP__
#define __DEEP_ENGINE_COMMON_HPP__

#include "DE/core/def.hpp"

#define _USE_MATH_DEFINES   // Nécessaire sous plateforme Microsoft pour définir M_PI.
#include <math.h>

namespace deep
{

    // ===== REMOVE REFERENCE ===== //

    /// @brief          Structure qui permet de récupérer le type sans les références.
    /// @tparam Type 
    template<class Type>
    struct rm_ref_s
    {
        using type = Type;
        using ctype = const Type;
    };

    /// @brief          Spécialisation de \ref rm_ref_s pour les références lvalue.
    /// @tparam Type    
    template<class Type>
    struct rm_ref_s<Type &>
    {
        using type = Type;
        using ctype = const Type &;
    };

    /// @brief          Spécialisation de \ref rm_ref_s pour les références rvalue.
    /// @tparam Type    
    template<class Type>
    struct rm_ref_s<Type &&>
    {
        using type = Type;
        using ctype = const Type &&;
    };

    /// @brief          Retire la référence d'un type.
    /// @tparam Type    Le type auquel retirer la référence.
    template<class Type>
    using rm_ref = typename rm_ref_s<Type>::type;    

    // ===== REMOVE CONST ===== //

    template<typename Type>
    struct rm_const_s
    {
        using type = Type;
    };

    template<typename Type>
    struct rm_const_s<const Type>
    {
        using type = Type;
    };

    template<typename Type>
    using rm_const = typename rm_const_s<Type>::type;

    template<typename Type>
    using add_const = const Type;

    // ===== REMOVE VOLATILE ===== //

    template<typename Type>
    struct rm_volatile_s
    {
        using type = Type;
    };

    template<typename Type>
    struct rm_volatile_s<volatile Type>
    {
        using type = Type;
    };

    template<typename Type>
    using rm_volatile = typename rm_volatile_s<Type>::type;

    // ===== REMOVE CONST VOLATILE ===== //

    template<typename Type>
    struct rm_const_volatile_s
    {
        using type = Type;

        template<template<typename> typename Fn>
        using apply = Fn<Type>;
    };

    template<typename Type>
    struct rm_const_volatile_s<const Type>
    {
        using type = Type;
    };

    template<typename Type>
    struct rm_const_volatile_s<volatile Type>
    {
        using type = Type;
    };

    template<typename Type>
    struct rm_const_volatile_s<const volatile Type>
    {
        using type = Type;
    };

    template<typename Type>
    using rm_const_volatile = typename rm_const_volatile_s<Type>::type;

    // ===== IS SAME ===== //

    template<typename, typename>
    inline constexpr bool is_same = false;

    template<typename Type>
    inline constexpr bool is_same<Type, Type> = true;

    // ===== IS POINTER ===== //

    template<typename>
    inline constexpr bool is_pointer = false;

    template<typename Type>
    inline constexpr bool is_pointer<Type *> = true;

    template<typename Type>
    inline constexpr bool is_pointer<const Type *> = true;

    template<typename Type>
    inline constexpr bool is_pointer<volatile Type *> = true;

    template<typename Type>
    inline constexpr bool is_pointer<const volatile Type *> = true;

    // ===== IS VOID ===== //

    template<typename Type>
    inline constexpr bool is_void = is_same<rm_const_volatile<Type>, void>;

    // ===== IS ENUM ===== //

    template<typename Type>
    inline constexpr bool is_enum = __is_enum(Type);

    // ===== UNDERLYING TYPE ===== //

    template<typename Type>
    using underlying_type = __underlying_type(Type);

    template<typename Base, typename Derived>
    inline constexpr bool is_base_of = __is_base_of(Base, Derived);

    template<typename Type>
    inline constexpr bool is_trivially_destructible = __is_trivially_destructible(Type);

    template<typename Enum>
    inline constexpr underlying_type<Enum> to_utype(Enum e)
    {
        return static_cast<underlying_type<Enum>>(e);
    }

    /// @brief          Cast une variable en \c rvalue.
    /// @tparam Type    Le type de la variable.
    /// @param value    La variable à passer en rvalue.
    /// @return         La référence rvalue de la variable.
    template<typename Type>
    constexpr rm_ref<Type> &&rvalue_cast(Type &&value) noexcept
    {
        return static_cast<rm_ref<Type>&&>(value);
    }

    // Indique que le template prend comme paramètre une 'taille'.
    template<size_t>
    struct mk_signed;

    // Ensuite il y a une spécification du template selon les différentes tailles.
    template<>
    struct mk_signed<1>
    {
        using type = int8_t;
    };

    template<>
    struct mk_signed<2>
    {
        using type = int16_t;
    };

    template<>
    struct mk_signed<4>
    {
        using type = int32_t;
    };

    template<>
    struct mk_signed<8>
    {
        using type = int64_t;
    };

    template<typename Type>
    using make_signed = typename mk_signed<sizeof(Type)>::type;

    /// @brief          Retourne la valeur de \ref val et met à jour sa valeur en prenant celle de \ref newVal.
    /// @tparam Type       
    /// @tparam K 
    /// @param val      La valeur à changer.
    /// @param newVal   La valeur à récupérer.
    /// @return         L'ancienne valeur de \ref val.
    template<typename Type, typename K = Type>
    Type exchange(Type &value, K &&newValue) noexcept
    {
        Type oldVal = static_cast<Type &&>(value);
        value = static_cast<K &&>(newValue);
        return oldVal;
    }

    DE_API uint64_t next_bit_number(uint64_t value, uint16_t fromBitNumber);

    inline constexpr float deg_to_rad(float degrees)
    {
        return (degrees * static_cast<float>(M_PI) / 180.0f);
    }

}


#endif