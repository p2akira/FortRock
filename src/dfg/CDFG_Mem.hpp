#ifndef _CDFG_MEM_H
#define _CDFG_MEM_H

#include <string>
#include <vector>
#include <memory>

#include "CDFG_Node.hpp"
#include "CDFG_Reg.hpp"
#include "CDFG_Wire.hpp"
#include "CDFG_Addr.hpp"

class CDFG_Addr; // 循環参照によるクラスの重複定義対応

/**
   @class CDFG_Mem
   @brief メモリを表すクラス
   配列やスタックなどのインタフェースモジュールが
   実際にアクセスするデータを表すクラス
*/
class CDFG_Mem : public CDFG_Node {
public:
  /** メモリの種類を表す定数 */
  enum class eMemType : unsigned {
    Array = 0, ///< 配列
      Ram,     ///< RAM
      Other,   ///< その他
      };

  enum class eDataType : unsigned {
    Integer = 0, ///< 整数型
      Float,     ///< 浮動小数点型
  };

  CDFG_Mem(const std::string & name,
           const unsigned & num_datas,
           const unsigned & word_length,
           const unsigned & num_w_port,
           const unsigned & num_r_port,
           const eMemType & mem_type,
           const eDataType & data_type,
           const bool & is_initialized,
           const unsigned & latency);

  virtual ~CDFG_Mem(void) {}

  // インタフェース依存の部分の出力関数 (Verilog HDL)
  virtual std::string init_string
  (const std::string & indent) = 0;   ///< 初期化部の出力
  virtual std::string define_string(void) = 0; ///< 宣言部の出力
  virtual std::string access_string
  (const std::shared_ptr<CDFG_Addr> & addr) = 0; ///< アクセス部(制御部も含む)の出力

  // getter
  unsigned get_num_w_port(void);
  unsigned get_num_r_port(void);
  const eMemType & get_mem_type(void);
  const unsigned & get_latency(void);
  std::shared_ptr<CDFG_Node> get_read_port(const unsigned & port_num);
  std::shared_ptr<CDFG_Node> get_write_port(const unsigned & port_num);

  // overrides
  virtual bool is_input(void) override final { return false; }
  virtual bool is_output(void) override final { return false; }

protected:
  unsigned _num_datas;   ///< メモリ上のデータ数
  unsigned _word_length; ///< ワード長
  std::vector<std::shared_ptr<CDFG_Reg> > _write_ports; ///< 書き込みポート
  std::vector<std::shared_ptr<CDFG_Wire> > _read_ports; ///< 読み込みポート
  const eMemType _mem_type;    ///< メモリの種類
  const bool _is_initialized;  ///< 初期化されるかどうか
  const eDataType _data_type;  ///< 格納されるデータ型
  const unsigned _latency;     ///< アクセスにかかるレイテンシ
};

#endif
