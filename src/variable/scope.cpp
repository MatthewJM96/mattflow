#include "stdafx.h"

#include "variable/scope.h"
#include "variable/variable.h"

void mfvar::ScopeTreeNodeInfoWriter::operator()(std::ostream& out, mfvar::Scope scope)
    const {
    auto& ident_table = mflit::IdentifierTable::get();

    std::string label = "";

    auto scope_map = var_table->get_scope_map(scope);
    if (scope_map) {
        for (auto [ident_idx, type] : *scope_map) {
            label += ident_table.get(ident_idx);
            label += "\n";
        }

        out << "[label=\"" + label + "\"]";
    }
}
