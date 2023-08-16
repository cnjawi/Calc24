add_rules("mode.debug", "mode.release")
set_languages('cxx17')
add_includedirs('cqltr/')  -- compile_commands.json 在保存时由 xmake 自动生成到 .vscode/

target('cqltr')
    set_kind('shared')
    add_files('cqltr/*.cpp')
    add_rules('utils.symbols.export_all')  -- 导出符号，即使代码中没有 __declspec(dllexport)

target('cqlt')
    set_kind('binary')
    add_deps('cqltr')
    add_files('console/*.cpp')
