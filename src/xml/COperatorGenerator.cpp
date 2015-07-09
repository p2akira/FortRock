#include "COperatorGenerator.hpp"

COperatorGenerator::COperatorGenerator(void){}
COperatorGenerator::~COperatorGenerator(void){}

/**
   演算器のインスタンス化 (moduleへ登録)
   @param[in] module_gen モジュールへの参照
 */
void COperatorGenerator::generate_operator
(std::shared_ptr<CModuleGenerator> & module_gen ) {
  for (auto i=0; i<this->_num_operator; ++i) {
    this->set_instance_name(this->_instance_name + std::to_string(i));
    auto ope = std::make_shared<CDFG_Operator>
      (this->_instance_name,
       this->_name,
       this->_latency,
       this->_type);

    // 入力ポートのインスタンス化
    for (auto & inode : this->_inode_info) {
      std::shared_ptr<CDFG_Node> node;
      if (inode->get_type() == CDFG_Node::eNode::CLK ||
          inode->get_type() == CDFG_Node::eNode::CE)
        node = module_gen->get_node(inode->get_type());
      else {
        node = std::make_shared<CDFG_Node>
          (this->_instance_name + '_' + inode->get_name(),
           inode->get_bit_width(),
           inode->get_is_signed(),
           inode->get_type());
      }
      ope->add_input_port(inode->get_name(),
                          node);
      module_gen->add_node(node);
    }

    // 出力ポートのインスタンス化
    for (auto & onode : this->_onode_info) {
      auto node = std::make_shared<CDFG_Node>
        (CDFG_Node(this->_instance_name + '_' + onode->get_name(),
                   onode->get_bit_width(),
                   onode->get_is_signed(),
                   onode->get_type()));
      ope->add_output_port(onode->get_name(),
                           node);
      module_gen->add_node(node);
    }

    // 演算器のインスタンス化
    module_gen->add_operator(ope);
  }
}

/**
   入出力ポートの情報を追加
   @param[in] name ポート名
   @param[in] bit_width ポートのビット幅
   @param[in] type ポートの種類
 */
void COperatorGenerator::add_port_info
(const std::string & name,
 const unsigned & bit_width,
 const CDFG_Node::eNode & type,
 const bool & is_signed) {
  auto node_data = std::make_shared<CNode_data>
    (name, bit_width, type, is_signed);

  if (type == CDFG_Node::eNode::REG ||
      type == CDFG_Node::eNode::CLK ||
      type == CDFG_Node::eNode::CE)
    this->_inode_info.push_back(node_data);
  else if (type == CDFG_Node::eNode::WIRE)
    this->_onode_info.push_back(node_data);
}

/**
   演算器の種類を設定
   @param[in] type 演算器の種類
 */
void COperatorGenerator::set_type
(const CDFG_Operator::eType & type) {
  this->_type = type;
}

/**
   演算器のレイテンシを設定
   @param[in] latency 演算器のレイテンシ
 */
void COperatorGenerator::set_latency
(const unsigned & latency) {
  this->_latency = latency;
}

/**
   演算器のモジュール名を設定
   @param[in] name 演算器のモジュール名
 */
void COperatorGenerator::set_name
(const std::string & name) {
  this->_name = name;
}

/**
   演算器のインスタンス名を設定
   @param[in] name 演算器のインスタンス名
 */
void COperatorGenerator::set_instance_name
(const std::string & name) {
  this->_instance_name = name;
}

/**
   インスタンス化する演算器の数を設定
   @param[in] num インスタンス化する演算器の数
 */
void COperatorGenerator::set_num_operator
(const unsigned & num) {
  this->_num_operator = num;
}