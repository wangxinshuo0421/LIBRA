/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 New York University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Adrian S.-W. Tam <adrian.sw.tam@gmail.com>
 */

#include <stdint.h>
#include <iostream>
#include "cn-header.h"
#include "ns3/buffer.h"
#include "ns3/address-utils.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("CnHeader");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CnHeader);

CnHeader::CnHeader (const uint16_t fid, uint8_t qIndex, uint8_t ecnbits, uint16_t qfb, uint16_t total)
  : m_fid(fid), m_qIndex(qIndex), m_qfb(qfb), m_ecnBits(ecnbits), m_total(total)
{
  //NS_LOG_LOGIC("CN got the flow id " << std::hex << m_fid.hi << "+" << m_fid.lo << std::dec);
}

/*
CnHeader::CnHeader (const uint16_t fid, uint8_t qIndex, uint8_t qfb)
	: m_fid(fid), m_qIndex(qIndex), m_qfb(qfb), m_ecnBits(0)
{
	//NS_LOG_LOGIC("CN got the flow id " << std::hex << m_fid.hi << "+" << m_fid.lo << std::dec);
}
*/

CnHeader::CnHeader ()
  : m_fid(), m_qIndex(), m_qfb(0), m_ecnBits(0)
{}

CnHeader::~CnHeader ()
{}

void CnHeader::SetFlow (const uint16_t fid)
{
  m_fid = fid;
}

void CnHeader::SetQindex (const uint8_t qIndex)
{
	m_qIndex = qIndex;
}

void CnHeader::SetQfb (uint16_t q)
{
  m_qfb = q;
}

void CnHeader::SetTotal (uint16_t total)
{
	m_total = total;
}

void CnHeader::SetECNBits (const uint8_t ecnbits)
{
	m_ecnBits = ecnbits;
}


uint16_t CnHeader::GetFlow () const
{
  return m_fid;
}

uint8_t CnHeader::GetQindex () const
{
	return m_qIndex;
}

uint16_t CnHeader::GetQfb () const
{
  return m_qfb;
}

uint16_t CnHeader::GetTotal() const
{
	return m_total;
}

uint8_t CnHeader::GetECNBits() const
{
	return m_ecnBits;
}

void CnHeader::SetSeq (const uint32_t seq){
	m_seq = seq;
}

uint32_t CnHeader::GetSeq () const{
	return m_seq;
}

void CnHeader::SetSEANextAddr (const uint32_t nextaddr){    // host order in
    m_nextaddr = nextaddr;
    m_nextaddr_ntoh = ((nextaddr&0xff) << 24) | (((nextaddr>>8)&0xff) << 16) | (((nextaddr>>16)&0xff) << 8) | ((nextaddr>>24)&0xff);
    // std::cout << "m_nextaddr_ntoh = " << m_nextaddr_ntoh << " nextaddr " << nextaddr << std::endl;
}
uint32_t CnHeader::GetSEANextAddr () const{
    return m_nextaddr;
}

void CnHeader::SetSEADstAddr (const uint32_t dstaddr){    // host order in
    m_dstaddr = dstaddr;
    m_dstaddr_ntoh = ((dstaddr>>24)&0xff) | (((dstaddr>>16)&0xff)<<8) | (((dstaddr>>8)&0xff)<<16) | (((dstaddr)&0xff)<<24);
}
uint32_t CnHeader::GetSEADstAddr () const{
    return m_dstaddr;
}

TypeId 
CnHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CnHeader")
    .SetParent<Header> ()
    .AddConstructor<CnHeader> ()
    ;
  return tid;
}
TypeId 
CnHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
void CnHeader::Print (std::ostream &os) const
{
  //m_fid.Print(os);
  os << "qIdx=" << (uint32_t)m_qIndex << " m_fid=" << (uint32_t)m_fid \
    //  << " nextaddr: " << (uint32_t)((m_nextaddr_ntoh>>24)&0xff) << "."<< (uint32_t)((m_nextaddr_ntoh>>16)&0xff) << "." << (uint32_t)((m_nextaddr_ntoh>>8)&0xff) << "."<< (uint32_t)((m_nextaddr_ntoh>>0)&0xff)\
    //  << " dstaddr: " << (uint32_t)((m_dstaddr_ntoh>>24)&0xff) << "."<< (uint32_t)((m_dstaddr_ntoh>>16)&0xff) << "." << (uint32_t)((m_dstaddr_ntoh>>8)&0xff) << "."<< (uint32_t)((m_dstaddr_ntoh>>0)&0xff)
     ;
}
uint32_t CnHeader::GetSerializedSize (void)  const
{
  return 16;
}
void CnHeader::Serialize (Buffer::Iterator start)  const
{
  start.WriteHtonU16(m_fid);
  start.WriteU8(m_qIndex);
  start.WriteU8(m_ecnBits);
  start.WriteHtonU32(m_seq);
  start.WriteHtonU32(m_nextaddr);
  start.WriteHtonU32(m_dstaddr);
  //NS_LOG_LOGIC("CN Seriealized as " << std::hex << hibyte << "+" << lobyte << std::dec);
}

uint32_t CnHeader::Deserialize (Buffer::Iterator start)
{
  m_fid = start.ReadNtohU16();
  m_qIndex = start.ReadU8();
  m_ecnBits = start.ReadU8();
  m_seq = start.ReadNtohU32();
  m_nextaddr = start.ReadNtohU32();
  m_dstaddr = start.ReadNtohU32();

  //m_qfb = static_cast<uint8_t>(lobyte>>24);
  //m_qIndex = lobyte & 0x00FFFFFFLLU;

  return GetSerializedSize ();
}


}; // namespace ns3
