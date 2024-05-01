#ifndef SEANET_HEADER_H
#define SEANET_HEADER_H

#include "ns3/header.h"

namespace ns3 {

class SeanetHeader : public Header 
{
public:

  /**
   * \brief Construct a SeanetHeader header.
   */
  SeanetHeader ();

  /**
   * \brief Destroy a SeanetHeader header.
   */
  virtual ~SeanetHeader ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual uint32_t GetSerializedSize (void) const;
  static uint32_t GetStaticSize (void);

  /**
   * \brief Set the extend addr carried by this SEANet packet
   *
   * \param protocol the protocol type being carried
   */
  void SetSEANextAddr (uint32_t nextaddr);
  void SetSEADstAddr (uint32_t dstaddr);

  /**
   * \brief Get the extend addr carried by this SEANet packet
   *
   * \return the extend addr being carried
   */
  uint32_t GetSEANextAddr (void);
  uint32_t GetSEADstAddr (void);

  uint32_t GetSEANextAddrHostOrder (void);
  uint32_t GetSEADstAddrHostOrder (void);

private:

  /**
   * \brief The extend addr of the packet
   */
  uint32_t m_seanet_next_addr;
  uint32_t m_seanet_dst_addr;
  uint32_t m_seanet_next_addr_ntoh;
  uint32_t m_seanet_dst_addr_ntoh;
};

} // namespace ns3


#endif /* SEANET_HEADER_H */
