#include <iostream>
#include "ns3/abort.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/header.h"
#include "seanet-header.h"

NS_LOG_COMPONENT_DEFINE ("SeanetHeader");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SeanetHeader);

SeanetHeader::SeanetHeader ()
{
}

SeanetHeader::~SeanetHeader ()
{
}

TypeId
SeanetHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SeanetHeader")
    .SetParent<Header> ()
    .AddConstructor<SeanetHeader> ()
  ;
  return tid;
}

TypeId
SeanetHeader::GetInstanceTypeId (void) const
{
    return GetTypeId ();
}

void 
SeanetHeader::Print (std::ostream &os) const
{
  os << "SeanetHeader:: next addr : " \
     << ((m_seanet_next_addr>>24)&0xff) << "." << ((m_seanet_next_addr>>16)&0xff) << "." << ((m_seanet_next_addr>>8)&0xff) << "." << (m_seanet_next_addr&0xff) \
     << " SeanetHeader:: dst addr : " 
     << ((m_seanet_dst_addr>>24)&0xff) << "." << ((m_seanet_dst_addr>>16)&0xff) << "." << ((m_seanet_dst_addr>>8)&0xff) << "." << (m_seanet_dst_addr&0xff) \
     ; 
}

uint32_t
SeanetHeader::GetSerializedSize (void) const
{
	return GetStaticSize();
}
uint32_t SeanetHeader::GetStaticSize (void){
	return 8;
}

void
SeanetHeader::Serialize (Buffer::Iterator start) const
{
    start.WriteHtonU32(m_seanet_next_addr);
    start.WriteHtonU32(m_seanet_dst_addr);
}

uint32_t
SeanetHeader::Deserialize (Buffer::Iterator start)
{
    m_seanet_next_addr = start.ReadNtohU32 ();
    m_seanet_dst_addr = start.ReadNtohU32 ();
    return GetSerializedSize ();
}

void
SeanetHeader::SetSEANextAddr (uint32_t nextaddr) // host order in
{
//   m_seanet_next_addr=((nextaddr&0xff) << 24) | (((nextaddr>>8)&0xff) << 16) | (((nextaddr>>16)&0xff) << 8) | ((nextaddr>>24)&0xff);
    m_seanet_next_addr = nextaddr;
}

void
SeanetHeader::SetSEADstAddr (uint32_t dstaddr) // host order in
{
    m_seanet_dst_addr = dstaddr;
}

uint32_t
SeanetHeader::GetSEANextAddr (void)
{
    return m_seanet_next_addr;
}

uint32_t
SeanetHeader::GetSEADstAddr (void)
{
    return m_seanet_dst_addr;
}

} // namespace ns3
