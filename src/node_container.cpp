#include <algorithm>
#include "node_container.h"
#include "type_id.h"


namespace galois::gparallel
{


    //register_node_op<NT>();
    //for (auto & node : _nodes) {
    //    _name_node_map[node_ptr->name()] = _this_node;
    //    _name_options_map[node_ptr->name()] = options;
    //}

static node_info * const PRODUCE_MARK = reinterpret_cast<node_info*>(0x1);
static node_info * const SOUT_MARK = reinterpret_cast<node_info*>(0x2);

bool node_container::init()
{
    // check nodes
    if (auto pos = std::find_if(_nodes.begin(), _nodes.end(), [](auto ptr){
       return ptr->_query_out.size() > 0 && ptr->_item_out.size() > 0;
    }); pos != _nodes.end()) {
        //log(FATAL, "can not process both query and item:%s", (*pos)->name().c_str());
        return false;
    }
    std::for_each(_nodes.begin(), _nodes.end(), [](auto ptr){
        //int node_user_id = find_node_user_id_by_name(ptr->name());  
        //ptr->set_node_user_id(node_user_id);
    });


    std::map< int, std::vector<std::shared_ptr<node_info>> > produce_data;
    std::map< int, std::vector<std::shared_ptr<node_info>> > sout_data;
    std::map< int64_t, std::shared_ptr<node_info> > out_data;
    for (auto node : _nodes) {
        for (auto & data : node->_data_out) {
            if (produce_data.count(data.id) && data.type != PRODUCE) {
                //log(FATAL, "can't mix OUTPUT/SOUT with PRODUCE: node(%s, %s) produce data[%d]: %s",
                //    produce_data[data.id][0]->name().c_str(), node->name().c_str(),
                //    data.id, typeid_manager<auto_type>::instance().name(data.id).c_str());
                return -1;
            }
            switch (data.type) {
                case OUTPUT:
                    if (out_data.count(data.id)) {
                        //log(FATAL, "more than one node(%s, %s) produce data[%d]: %s",
                        //    out_data[data.id]->name().c_str(), node->name().c_str(),
                        //    data.id, typeid_manager<auto_type>::instance().name(data.id).c_str());
                        return -1;
                    }
                    out_data[data.id] = node;
                    break;
                case SOUT:
                    out_data[data.id] = std::shared_ptr<node_info>(SOUT_MARK, [](auto){});
                    sout_data[data.id].push_back(node);
                    break;
                case PRODUCE:
                    if (out_data.count(data.id) && out_data[data.id].get() != PRODUCE_MARK) {
                        //log(FATAL, "can't mix OUT/SOUT with PRODUCE: node(%s, %s) produce data[%d]: %s",
                        //    out_data[data.id].get() == SOUT_MARK ?
                        //        sout_data[data.id][0]->name().c_str() : out_data[data.id]->name().c_str(),
                        //    node->name().c_str(), data.id, typeid_manager<auto_type>::instance().name(data.id).c_str());
                        return -1;
                    }
                    out_data[data.id] = std::shared_ptr<node_info>(PRODUCE_MARK, [](auto){});
                    produce_data[data.id].push_back(node);
                    break;
                case INPUT:
                case NONE:
                default:
                    //log(FATAL, "invaild here");
                    break;
            }
        }
    }
}
   


}