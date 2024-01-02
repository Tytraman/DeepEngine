#ifndef __DEEP_ENGINE_COMMON_HPP__
#define __DEEP_ENGINE_COMMON_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>

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
    inline constexpr bool isSame = false;

    template<typename Type>
    inline constexpr bool isSame<Type, Type> = true;

    // ===== IS POINTER ===== //

    template<typename>
    inline constexpr bool isPointer = false;

    template<typename Type>
    inline constexpr bool isPointer<Type *> = true;

    template<typename Type>
    inline constexpr bool isPointer<const Type *> = true;

    template<typename Type>
    inline constexpr bool isPointer<volatile Type *> = true;

    template<typename Type>
    inline constexpr bool isPointer<const volatile Type *> = true;

    // ===== IS VOID ===== //

    template<typename Type>
    inline constexpr bool isVoid = isSame<rm_const_volatile<Type>, void>;

    // ===== IS ENUM ===== //

    template<typename Type>
    inline constexpr bool isEnum = __is_enum(Type);

    // ===== UNDERLYING TYPE ===== //

    template<typename Type>
    using underlying_type = __underlying_type(Type);


    template<typename Enum>
    inline constexpr underlying_type<Enum> toUType(Enum e)
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

    /// @brief          Retourne la valeur de \ref val et met à jour sa valeur en prenant celle de \ref newVal.
    /// @tparam Type       
    /// @tparam K 
    /// @param val      La valeur à changer.
    /// @param newVal   La valeur à récupérer.
    /// @return         L'ancienne valeur de \ref val.
    template<typename Type, typename K = Type>
    Type exchange(Type &val, K &&newVal) noexcept
    {
        Type oldVal = static_cast<Type &&>(val);
        val = static_cast<K &&>(newVal);
        return oldVal;
    }

    

}


#endif