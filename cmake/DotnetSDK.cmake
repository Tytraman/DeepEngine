function(deep_compile_cs)
    set(DEEP_MANAGED "DeepManaged")

    set(DEEP_MANAGED_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/DeepManaged/DeepManaged.csproj"
        "${CMAKE_CURRENT_LIST_DIR}/DeepManaged/EngineAPI.cs")

    find_program(DOTNET_EXECUTABLE dotnet)

    if(NOT DOTNET_EXECUTABLE)
        message(FATAL_ERROR "dotnet CLI not found. Please install .NET SDK.")
    endif()

    # Récupère la version installée de dotnet.
    execute_process(
        COMMAND ${DOTNET_EXECUTABLE} --version
        OUTPUT_VARIABLE DOTNET_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    message(STATUS ".NET SDK found : ${DOTNET_VERSION}")

    add_custom_target(
        DeepManagedBuild ALL
        COMMAND ${DOTNET_EXECUTABLE} build 
            "${CMAKE_CURRENT_LIST_DIR}/${DEEP_MANAGED}/${DEEP_MANAGED}.csproj"
            -o "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        SOURCES ${DEEP_MANAGED_SOURCES}
        COMMENT "Compiling C# scripts..."
        VERBATIM
        USES_TERMINAL   # Exécute la commande dans le terminal parent : permet d'afficher la sortie de la commande dans le terminal.
    )

    add_dependencies(DeepEngine DeepManagedBuild)   # Indique que 'DeepEngineManaged' doit être compilé AVANT 'DeepEngine'.
endfunction()