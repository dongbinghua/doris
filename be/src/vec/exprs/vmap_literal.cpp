// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "vec/exprs/vmap_literal.h"

//insert into table_map values ({'name':'zhangsan', 'gender':'male'}), ({'name':'lisi', 'gender':'female'});
namespace doris::vectorized {

Status VMapLiteral::prepare(RuntimeState* state, const RowDescriptor& row_desc,
                            VExprContext* context) {
    DCHECK_EQ(type().children.size(), 2) << "map children type not 2";

    RETURN_IF_ERROR_OR_PREPARED(VExpr::prepare(state, row_desc, context));
    // map-field should contain two vector field for keys and values
    Field map = Map();
    Field keys = Array();
    Field values = Array();
    // each child is slot with key1, value1, key2, value2...
    for (int idx = 0; idx < _children.size(); ++idx) {
        Field item;
        ColumnPtrWrapper* const_col_wrapper = nullptr;
        RETURN_IF_ERROR(_children[idx]->get_const_col(context, &const_col_wrapper));
        const_col_wrapper->column_ptr->get(0, item);

        if ((idx & 1) == 0) {
            keys.get<Array>().push_back(item);
        } else {
            values.get<Array>().push_back(item);
        }
    }
    map.get<Map>().push_back(keys);
    map.get<Map>().push_back(values);

    _column_ptr = _data_type->create_column_const(1, map);
    return Status::OK();
}

} // namespace doris::vectorized
