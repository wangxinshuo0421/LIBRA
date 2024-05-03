#include <ns3/simulator.h>
#include <ns3/seq-ts-header.h>
#include <ns3/udp-header.h>
#include <ns3/ipv4-header.h>
#include "ns3/ppp-header.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/data-rate.h"
#include "ns3/pointer.h"
#include "rdma-hw.h"
#include "ppp-header.h"
#include "qbb-header.h"
#include "cn-header.h"
#include "seanet-header.h"
#include <random>

namespace ns3{

TypeId RdmaHw::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::RdmaHw")
        .SetParent<Object> ()
        .AddAttribute("MinRate",
                "Minimum rate of a throttled flow",
                DataRateValue(DataRate("100Mb/s")),
                MakeDataRateAccessor(&RdmaHw::m_minRate),
                MakeDataRateChecker())
        .AddAttribute("Mtu",
                "Mtu.",
                UintegerValue(1000),
                MakeUintegerAccessor(&RdmaHw::m_mtu),
                MakeUintegerChecker<uint32_t>())
        .AddAttribute ("CcMode",
                "which mode of DCQCN is running",
                UintegerValue(0),
                MakeUintegerAccessor(&RdmaHw::m_cc_mode),
                MakeUintegerChecker<uint32_t>())
        .AddAttribute("NACK Generation Interval",
                "The NACK Generation interval",
                DoubleValue(500.0),
                MakeDoubleAccessor(&RdmaHw::m_nack_interval),
                MakeDoubleChecker<double>())
        .AddAttribute("L2ChunkSize",
                "Layer 2 chunk size. Disable chunk mode if equals to 0.",
                UintegerValue(0),
                MakeUintegerAccessor(&RdmaHw::m_chunk),
                MakeUintegerChecker<uint32_t>())
        .AddAttribute("L2AckInterval",
                "Layer 2 Ack intervals. Disable ack if equals to 0.",
                UintegerValue(0),
                MakeUintegerAccessor(&RdmaHw::m_ack_interval),
                MakeUintegerChecker<uint32_t>())
        .AddAttribute("L2BackToZero",
                "Layer 2 go back to zero transmission.",
                BooleanValue(false),
                MakeBooleanAccessor(&RdmaHw::m_backto0),
                MakeBooleanChecker())
        .AddAttribute("EwmaGain",
                "Control gain parameter which determines the level of rate decrease",
                DoubleValue(1.0 / 16),
                MakeDoubleAccessor(&RdmaHw::m_g),
                MakeDoubleChecker<double>())
        .AddAttribute ("RateOnFirstCnp",
                "the fraction of rate on first CNP",
                DoubleValue(1.0),
                MakeDoubleAccessor(&RdmaHw::m_rateOnFirstCNP),
                MakeDoubleChecker<double> ())
        .AddAttribute("ClampTargetRate",
                "Clamp target rate.",
                BooleanValue(false),
                MakeBooleanAccessor(&RdmaHw::m_EcnClampTgtRate),
                MakeBooleanChecker())
        .AddAttribute("RPTimer",
                "The rate increase timer at RP in microseconds",
                DoubleValue(1500.0),
                MakeDoubleAccessor(&RdmaHw::m_rpgTimeReset),
                MakeDoubleChecker<double>())
        .AddAttribute("RateDecreaseInterval",
                "The interval of rate decrease check",
                DoubleValue(4.0),
                MakeDoubleAccessor(&RdmaHw::m_rateDecreaseInterval),
                MakeDoubleChecker<double>())
        .AddAttribute("FastRecoveryTimes",
                "The rate increase timer at RP",
                UintegerValue(5),
                MakeUintegerAccessor(&RdmaHw::m_rpgThreshold),
                MakeUintegerChecker<uint32_t>())
        .AddAttribute("AlphaResumInterval",
                "The interval of resuming alpha",
                DoubleValue(55.0),
                MakeDoubleAccessor(&RdmaHw::m_alpha_resume_interval),
                MakeDoubleChecker<double>())
        .AddAttribute("RateAI",
                "Rate increment unit in AI period",
                DataRateValue(DataRate("5Mb/s")),
                MakeDataRateAccessor(&RdmaHw::m_rai),
                MakeDataRateChecker())
        .AddAttribute("RateHAI",
                "Rate increment unit in hyperactive AI period",
                DataRateValue(DataRate("50Mb/s")),
                MakeDataRateAccessor(&RdmaHw::m_rhai),
                MakeDataRateChecker())
        .AddAttribute("VarWin",
                "Use variable window size or not",
                BooleanValue(false),
                MakeBooleanAccessor(&RdmaHw::m_var_win),
                MakeBooleanChecker())
        .AddAttribute("FastReact",
                "Fast React to congestion feedback",
                BooleanValue(true),
                MakeBooleanAccessor(&RdmaHw::m_fast_react),
                MakeBooleanChecker())
        .AddAttribute("MiThresh",
                "Threshold of number of consecutive AI before MI",
                UintegerValue(5),
                MakeUintegerAccessor(&RdmaHw::m_miThresh),
                MakeUintegerChecker<uint32_t>())
        .AddAttribute("TargetUtil",
                "The Target Utilization of the bottleneck bandwidth, by default 95%",
                DoubleValue(0.95),
                MakeDoubleAccessor(&RdmaHw::m_targetUtil),
                MakeDoubleChecker<double>())
        .AddAttribute("UtilHigh",
                "The upper bound of Target Utilization of the bottleneck bandwidth, by default 98%",
                DoubleValue(0.98),
                MakeDoubleAccessor(&RdmaHw::m_utilHigh),
                MakeDoubleChecker<double>())
        .AddAttribute("RateBound",
                "Bound packet sending by rate, for test only",
                BooleanValue(true),
                MakeBooleanAccessor(&RdmaHw::m_rateBound),
                MakeBooleanChecker())
        .AddAttribute("MultiRate",
                "Maintain multiple rates in HPCC",
                BooleanValue(true),
                MakeBooleanAccessor(&RdmaHw::m_multipleRate),
                MakeBooleanChecker())
        .AddAttribute("SampleFeedback",
                "Whether sample feedback or not",
                BooleanValue(false),
                MakeBooleanAccessor(&RdmaHw::m_sampleFeedback),
                MakeBooleanChecker())
        .AddAttribute("TimelyAlpha",
                "Alpha of TIMELY",
                DoubleValue(0.875),
                MakeDoubleAccessor(&RdmaHw::m_tmly_alpha),
                MakeDoubleChecker<double>())
        .AddAttribute("TimelyBeta",
                "Beta of TIMELY",
                DoubleValue(0.8),
                MakeDoubleAccessor(&RdmaHw::m_tmly_beta),
                MakeDoubleChecker<double>())
        .AddAttribute("TimelyTLow",
                "TLow of TIMELY (ns)",
                UintegerValue(50000),
                MakeUintegerAccessor(&RdmaHw::m_tmly_TLow),
                MakeUintegerChecker<uint64_t>())
        .AddAttribute("TimelyTHigh",
                "THigh of TIMELY (ns)",
                UintegerValue(500000),
                MakeUintegerAccessor(&RdmaHw::m_tmly_THigh),
                MakeUintegerChecker<uint64_t>())
        .AddAttribute("TimelyMinRtt",
                "MinRtt of TIMELY (ns)",
                UintegerValue(20000),
                MakeUintegerAccessor(&RdmaHw::m_tmly_minRtt),
                MakeUintegerChecker<uint64_t>())
        .AddAttribute("DctcpRateAI",
                "DCTCP's Rate increment unit in AI period",
                DataRateValue(DataRate("1000Mb/s")),
                MakeDataRateAccessor(&RdmaHw::m_dctcp_rai),
                MakeDataRateChecker())
        .AddAttribute("PintSmplThresh",
                "PINT's sampling threshold in rand()%65536",
                UintegerValue(65536),
                MakeUintegerAccessor(&RdmaHw::pint_smpl_thresh),
                MakeUintegerChecker<uint32_t>())
        .AddAttribute("MaxRtt",
                "Max Rtt of the network",
                UintegerValue(9000),
                MakeUintegerAccessor(&RdmaHw::m_maxRtt),
                MakeUintegerChecker<uint64_t>())
        ;
    return tid;
}

RdmaHw::RdmaHw(){
}

void RdmaHw::SetNode(Ptr<Node> node){
    m_node = node;
}

void RdmaHw::SetNextIpTable(std::map<uint32_t, std::vector<Ipv4Address> > _next_ip_table){
    next_ip_table = _next_ip_table;
    for (auto it = _next_ip_table.begin(); it != _next_ip_table.end(); ++it) {
        for (uint16_t i = 0; i < it->second.size(); i++){
            next_ip_weight_table[it->first].push_back(PATH_AVG_WEIGHT);
            next_ip_timestamp[it->first].push_back(0);
        }
    }
}

void RdmaHw::Setup(QpCompleteCallback cb){
    for (uint32_t i = 0; i < m_nic.size(); i++){
        Ptr<QbbNetDevice> dev = m_nic[i].dev;
        if (dev == NULL)
            continue;
        // share data with NIC
        dev->m_rdmaEQ->m_qpGrp = m_nic[i].qpGrp;
        // setup callback
        dev->m_rdmaReceiveCb = MakeCallback(&RdmaHw::Receive, this);
        dev->m_rdmaLinkDownCb = MakeCallback(&RdmaHw::SetLinkDown, this);
        dev->m_rdmaPktSent = MakeCallback(&RdmaHw::PktSent, this);
        // config NIC
        dev->m_rdmaEQ->m_rdmaGetNxtPkt = MakeCallback(&RdmaHw::GetNxtPacket, this);
    }
    ScheduleRetransWin();
    // setup qp complete callback
    m_qpCompleteCallback = cb;
}

void RdmaHw::ScheduleRetransWin() {
    uint32_t qp_nums = 0;
    for (uint32_t i = 0; i < m_nic.size(); i++){
        Ptr<QbbNetDevice> dev = m_nic[i].dev;
        if (dev == NULL)
            continue;
        if (dev->m_rdmaEQ->m_qpGrp->GetN() > 0) {
            qp_nums = qp_nums + dev->m_rdmaEQ->m_qpGrp->GetN();
            dev->TriggerTransmit();
        }
    }

    if ((Simulator::Now() < Simulator::GetMaximumSimulationTime())) {
        if (Simulator::Now() == 0)
            Simulator::Schedule(NanoSeconds(2000000000), &RdmaHw::ScheduleRetransWin, this);
        else if ((Simulator::Now() > 2100000000) && (qp_nums == 0))
            return;
        else
            Simulator::Schedule(NanoSeconds(m_maxRtt>0? m_maxRtt : 2000000000), &RdmaHw::ScheduleRetransWin, this);
    }
}

uint32_t RdmaHw::GetNicIdxOfQp(Ptr<RdmaQueuePair> qp){
    auto &v = m_rtTable[qp->dip.Get()];
    if (v.size() > 0){
        return v[qp->GetHash() % v.size()];
    }else{
        NS_ASSERT_MSG(false, "We assume at least one NIC is alive");
    }
}
uint64_t RdmaHw::GetQpKey(uint32_t dip, uint16_t sport, uint16_t pg){
    return ((uint64_t)dip << 32) | ((uint64_t)sport << 16) | (uint64_t)pg;
}
Ptr<RdmaQueuePair> RdmaHw::GetQp(uint32_t dip, uint16_t sport, uint16_t pg){
    uint64_t key = GetQpKey(dip, sport, pg);
    auto it = m_qpMap.find(key);
    if (it != m_qpMap.end())
        return it->second;
    return NULL;
}
void RdmaHw::AddQueuePair(uint64_t size, uint16_t pg, Ipv4Address sip, Ipv4Address dip, uint16_t sport, uint16_t dport, uint16_t dstId, uint32_t win, uint64_t baseRtt, Callback<void> notifyAppFinish){
    // create qp
    Ptr<RdmaQueuePair> qp = CreateObject<RdmaQueuePair>(pg, sip, dip, sport, dport, dstId);
    qp->SetSize(size);
    qp->SetWin(win);
    qp->SetBaseRtt(baseRtt);
    qp->SetVarWin(m_var_win);
    qp->SetAppNotifyCallback(notifyAppFinish);

    // add qp
    uint32_t nic_idx = GetNicIdxOfQp(qp);
    m_nic[nic_idx].qpGrp->AddQp(qp);
    uint64_t key = GetQpKey(dip.Get(), sport, pg);
    m_qpMap[key] = qp;

    // set init variables
    DataRate m_bps = m_nic[nic_idx].dev->GetDataRate();
    qp->m_rate = m_bps;
    qp->m_max_rate = m_bps;
    if (m_cc_mode == 1){
        qp->mlx.m_targetRate = m_bps;
    }else if (m_cc_mode == 3){
        qp->hp.m_curRate = m_bps;
        if (m_multipleRate){
            for (uint32_t i = 0; i < IntHeader::maxHop; i++)
                qp->hp.hopState[i].Rc = m_bps;
        }
    }else if (m_cc_mode == 7){
        qp->tmly.m_curRate = m_bps;
    }else if (m_cc_mode == 10){
        qp->hpccPint.m_curRate = m_bps;
    }

    // Notify Nic
    m_nic[nic_idx].dev->NewQp(qp);
}

void RdmaHw::DeleteQueuePair(Ptr<RdmaQueuePair> qp){
    // remove qp from the m_qpMap
    uint64_t key = GetQpKey(qp->dip.Get(), qp->sport, qp->m_pg);
    m_qpMap.erase(key);
}

Ptr<RdmaRxQueuePair> RdmaHw::GetRxQp(uint32_t sip, uint32_t dip, uint16_t sport, uint16_t dport, uint16_t pg, bool create){
    uint64_t key = ((uint64_t)dip << 32) | ((uint64_t)pg << 16) | (uint64_t)dport;
    auto it = m_rxQpMap.find(key);
    if (it != m_rxQpMap.end())
        return it->second;
    if (create){
        // create new rx qp
        Ptr<RdmaRxQueuePair> q = CreateObject<RdmaRxQueuePair>();
        // init the qp
        q->sip = sip;
        q->dip = dip;
        q->sport = sport;
        q->dport = dport;
        q->m_ecn_source.qIndex = pg;
        // store in map
        m_rxQpMap[key] = q;
        return q;
    }
    return NULL;
}
uint32_t RdmaHw::GetNicIdxOfRxQp(Ptr<RdmaRxQueuePair> q){
    auto &v = m_rtTable[q->dip];
    if (v.size() > 0){
        return v[q->GetHash() % v.size()];
    }else{
        NS_ASSERT_MSG(false, "We assume at least one NIC is alive");
    }
}
void RdmaHw::DeleteRxQp(uint32_t dip, uint16_t pg, uint16_t dport){
    uint64_t key = ((uint64_t)dip << 32) | ((uint64_t)pg << 16) | (uint64_t)dport;
    m_rxQpMap.erase(key);
}

int RdmaHw::ReceiveUdp(Ptr<Packet> p, CustomHeader &ch){
    uint8_t ecnbits = ch.GetIpv4EcnBits();

    uint32_t payload_size = p->GetSize() - ch.GetSerializedSize() ;

    // TODO find corresponding rx queue pair
    Ptr<RdmaRxQueuePair> rxQp = GetRxQp(ch.dip, ch.sip, ch.udp.dport, ch.udp.sport, ch.udp.pg, true);
    if (ecnbits != 0){
        rxQp->m_ecn_source.ecnbits |= ecnbits;
        rxQp->m_ecn_source.qfb++;
    }
    rxQp->m_ecn_source.total++;
    rxQp->m_milestone_rx = m_ack_interval;
    
    //generate ACK; Disorderly direct writing
    qbbHeader seqh;
    seqh.SetSeq(ch.udp.seq+payload_size);
    seqh.SetPG(ch.udp.pg);
    seqh.SetSport(ch.udp.dport);
    seqh.SetDport(ch.udp.sport);
    seqh.SetIntHeader(ch.udp.ih);
    if (ecnbits)
        seqh.SetCnp();

    Ptr<Packet> newp = Create<Packet>(std::max(60-14-20-(int)seqh.GetSerializedSize(), 0));
    newp->AddHeader(seqh);

    Ipv4Header head;	// Prepare IPv4 header
    head.SetDestination(Ipv4Address(ch.sip));
    head.SetSource(Ipv4Address(ch.dip));
    head.SetProtocol(1 ? 0xFC : 0xFD); //ack=0xFC nack=0xFD
    head.SetTtl(64);
    head.SetPayloadSize(newp->GetSize());
    head.SetIdentification(rxQp->m_ipid++);

    newp->AddHeader(head);
    AddHeader(newp, 0x800);	// Attach PPP header
    // send
    uint32_t nic_idx = GetNicIdxOfRxQp(rxQp);
    m_nic[nic_idx].dev->RdmaEnqueueHighPrioQ(newp);
    m_nic[nic_idx].dev->TriggerTransmit();

    #ifdef RDMA_HW_DEBUG_LOG
    std::cout << "ReceiveUdp:: " \
            << " pkt_dst: " << (ch.dip>>24) << "." << ((ch.dip>>16)&0xff) << "." << ((ch.dip>>8)&0xff) << "." << ((ch.dip)&0xff) \
            << "; src_ip " << (ch.sip>>24) << "." << ((ch.sip>>16)&0xff) << "." << ((ch.sip>>8)&0xff) << "." << ((ch.sip)&0xff) \
            << "; leaf rcv addr " << ((ch.udp.nextaddr)&0xff) << "." << ((ch.udp.nextaddr>>8)&0xff) << "." << ((ch.udp.nextaddr>>16)&0xff) << "." << (ch.udp.nextaddr>>24) \
            << "; rcv udp seq: " << ch.udp.seq \
            << "; send ack seq: " << ch.udp.seq+payload_size \
            << "; send ack pg: " << ch.udp.pg \
            << " now time " << Simulator::Now().GetTimeStep() \
            << std::endl;
    #endif
    return 0;
}

bool RdmaHw::TransferWeight(uint32_t dst_id, uint16_t congestion_path_id) {
    uint64_t now_time = Simulator::Now().GetTimeStep();
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(0, 40);
    uint16_t rand_offset = distr(eng);
    if (next_ip_timestamp[dst_id].size() == 0) {
        std::cout << "EEROR DST IP IN TransferWeight: dst_id " << dst_id << std::endl;
        return false;
    }

    next_ip_timestamp[dst_id][congestion_path_id] = now_time + m_weight_lock_period;

    if (next_ip_weight_table[dst_id][congestion_path_id] == 0) {   // no weight to transfer; Received excessive cnp
        return true;
    }

    uint16_t transferred_weight = 2;
    next_ip_weight_table[dst_id][congestion_path_id] -= transferred_weight;

    // get the number of available paths
    std::vector<uint16_t> avail_paths;
    for (uint16_t i = 0; i < next_ip_timestamp[dst_id].size(); i++) {
        if (next_ip_timestamp[dst_id][i] <= now_time) {
            avail_paths.push_back(i);
        }
    }

    if (avail_paths.size() < 1) {   // no available path to increase weight
        return false;
    }

    next_ip_weight_table[dst_id][rand_offset%avail_paths.size()] += transferred_weight;

    return true;    
}

int RdmaHw::ReceiveCnp(Ptr<Packet> p, CustomHeader &ch){
    // QCN on NIC
    // This is a Congestion signal
    // Then, extract data from the congestion packet.
    uint32_t qIndex = ch.cnp.qIndex;
    if (qIndex == 1){		//DCTCP
        std::cout << "TCP--ignore\n";
        return 0;
    }
    uint16_t udpport = ch.cnp.fid; // corresponds to the sport
    uint8_t ecnbits = ch.cnp.ecnBits;

    uint32_t i;
    // get qp
    Ptr<RdmaQueuePair> qp = GetQp(ch.cnp.dstaddr, udpport, qIndex);
    
    if (qp == NULL) {
        #ifdef RDMA_HW_DEBUG_LOG
        std::cout << "ERROR: QCN NIC cannot find the flow" << " dst ip " \
                << (ch.cnp.dstaddr>>24) << "." << ((ch.cnp.dstaddr>>16)&0xff) << "." << ((ch.cnp.dstaddr>>8)&0xff) << "." << ((ch.cnp.dstaddr)&0xff) \
                << " udpport " << udpport << " qIndex " << qIndex << std::endl;
        #endif
        return 0;
    }
    // get nic
    uint32_t nic_idx = GetNicIdxOfQp(qp);
    Ptr<QbbNetDevice> dev = m_nic[nic_idx].dev;
    uint16_t dst_id = qp->dst_id;
    uint32_t congestion_next_ip = ch.cnp.nextaddr;
    for (uint16_t i = 0; i < next_ip_table[dst_id].size(); i++) {
        if (congestion_next_ip == next_ip_table[dst_id][i].Get()) {
            bool path_available = TransferWeight(dst_id, i);
            break;
        }
    }

    #ifdef RDMA_HW_DEBUG_LOG
    std::cout << "ReceiveCnp::" \
            << "  pkt_dst: " << (ch.dip>>24) << "." << ((ch.dip>>16)&0xff) << "." << ((ch.dip>>8)&0xff) << "." << ((ch.dip)&0xff) \
            << "; src_ip " << (ch.sip>>24) << "." << ((ch.sip>>16)&0xff) << "." << ((ch.sip>>8)&0xff) << "." << ((ch.sip)&0xff) \
            << "; next_ip " << ((ch.cnp.nextaddr)&0xff) << "." << ((ch.cnp.nextaddr>>8)&0xff) << "." << ((ch.cnp.nextaddr>>16)&0xff) << "." << (ch.cnp.nextaddr>>24) \
            << "; rcv ack seq: " << ch.cnp.seq \
            << "; rcv ack port: " << udpport \
            << "; rcv qIndex: " << qIndex \
            << "; now time " << Simulator::Now().GetTimeStep() \
            << std::endl;
    p->Print(std::cout);
    std::cout << std::endl;
    #endif
    return 0;
}

int RdmaHw::ReceiveAck(Ptr<Packet> p, CustomHeader &ch){
    uint16_t qIndex = ch.ack.pg;
    uint16_t port = ch.ack.dport;
    uint32_t seq = ch.ack.seq;
    uint8_t cnp = (ch.ack.flags >> qbbHeader::FLAG_CNP) & 1;
    bool valid_ack = false, move_win = false;
    Ptr<RdmaQueuePair> qp = GetQp(ch.sip, port, qIndex);
    #ifdef RDMA_HW_DEBUG_LOG
    std::cout << "ReceiveAck1:: " \
            << " pkt_dst: " << (ch.dip>>24) << "." << ((ch.dip>>16)&0xff) << "." << ((ch.dip>>8)&0xff) << "." << ((ch.dip)&0xff) \
            << "; src_ip " << (ch.sip>>24) << "." << ((ch.sip>>16)&0xff) << "." << ((ch.sip>>8)&0xff) << "." << ((ch.sip)&0xff) \
            << "; rcv ack seq: " << ch.udp.seq \
            << "; rcv ack port: " << port \
            << "; rcv qIndex: " << qIndex \
            << " now time " << Simulator::Now().GetTimeStep() \
            << std::endl;
    #endif

    if (qp == NULL){
        std::cout << "ERROR: " << "node:" << m_node->GetId() << ' ' << (ch.l3Prot == 0xFC ? "ACK" : "NACK") << " NIC cannot find the flow\n";
        return 0;
    }
    for (auto it = qp->m_outstanding_pkts.begin(); it != qp->m_outstanding_pkts.end(); ++it) {
        if ((it->expect_ack == seq) && (it->acked == false)) {   // valid ack, delete this inflight pkt map
            it->acked = true;
            #ifdef RDMA_HW_DEBUG_LOG
            std::cout << "ReceiveAck2 : "\
                      << "expect_ack " << it->expect_ack << " rcv_ack " << seq << " qp->snd_una " << qp->snd_una << " qp->snd_nxt " << qp->snd_nxt << " it->acked " << it->acked \
                      << std::endl;
            #endif
            if (it == qp->m_outstanding_pkts.begin()) {
                move_win = true;
                do {
                    if (it->acked == true) {
                        seq = it->expect_ack;
                        qp->m_outstanding_pkts.erase(it);
                    } else 
                        break;
                } while(it != qp->m_outstanding_pkts.end()); // Eliminate a batch of consecutive acked bitmaps
            }
            valid_ack = true;
            break;
        }
    }
    if (!valid_ack){    // This is a duplicate ack
        return 0;
    }

    uint32_t nic_idx = GetNicIdxOfQp(qp);
    Ptr<QbbNetDevice> dev = m_nic[nic_idx].dev;
    if (m_ack_interval == 0)
        std::cout << "ERROR: shouldn't receive ack\n";
    else if(move_win) {
        if (!m_backto0){
            qp->Acknowledge(seq);
        }else {
            uint32_t goback_seq = seq / m_chunk * m_chunk;
            qp->Acknowledge(goback_seq);
        }
        #ifdef RDMA_HW_DEBUG_LOG
        std::cout << "ReceiveAck3 move_win :: snd_una " << qp->snd_una << " snd_nxt " << qp->snd_nxt << " " << qp->m_outstanding_pkts.size() << std::endl;
        #endif
        if (qp->IsFinished()){  // TODO: add inflight empty judge
            QpComplete(qp);
        }
    }

    // handle cnp
    if (cnp){
        if (m_cc_mode == 1){ // mlx version
            cnp_received_mlx(qp);
        } 
    }

    if (m_cc_mode == 3){
        HandleAckHp(qp, p, ch);
    }else if (m_cc_mode == 7){
        HandleAckTimely(qp, p, ch);
    }else if (m_cc_mode == 8){
        HandleAckDctcp(qp, p, ch);
    }else if (m_cc_mode == 10){
        HandleAckHpPint(qp, p, ch);
    }
    // ACK may advance the on-the-fly window, allowing more packets to send
    dev->TriggerTransmit();
    return 0;
}

int RdmaHw::Receive(Ptr<Packet> p, CustomHeader &ch){
    if (ch.l3Prot == 0x11){ // UDP
        ReceiveUdp(p, ch);
    }else if (ch.l3Prot == 0xFF){ // CNP
        ReceiveCnp(p, ch);
    }else if (ch.l3Prot == 0xFD){ // NACK
        ReceiveAck(p, ch);
    }else if (ch.l3Prot == 0xFC){ // ACK
        ReceiveAck(p, ch);
    }
    return 0;
}

int RdmaHw::ReceiverCheckSeq(uint32_t seq, Ptr<RdmaRxQueuePair> q, uint32_t size){
    uint32_t expected = q->ReceiverNextExpectedSeq;
    if (seq == expected){
        q->ReceiverNextExpectedSeq = expected + size;
        if (q->ReceiverNextExpectedSeq >= q->m_milestone_rx){
            q->m_milestone_rx += m_ack_interval;
            return 1; //Generate ACK
        }else if (q->ReceiverNextExpectedSeq % m_chunk == 0){
            return 1;
        }else {
            return 5;
        }
    } else if (seq > expected) {
        // Generate NACK
        if (Simulator::Now() >= q->m_nackTimer || q->m_lastNACK != expected){
            q->m_nackTimer = Simulator::Now() + MicroSeconds(m_nack_interval);
            q->m_lastNACK = expected;
            if (m_backto0){
                q->ReceiverNextExpectedSeq = q->ReceiverNextExpectedSeq / m_chunk*m_chunk;
            }
            return 2;
        }else
            return 4;
    }else {
        // Duplicate. 
        return 3;
    }
}
void RdmaHw::AddHeader (Ptr<Packet> p, uint16_t protocolNumber){
    PppHeader ppp;
    ppp.SetProtocol (EtherToPpp (protocolNumber));
    p->AddHeader (ppp);
}
uint16_t RdmaHw::EtherToPpp (uint16_t proto){
    switch(proto){
        case 0x0800: return 0x0021;   //IPv4
        case 0x86DD: return 0x0057;   //IPv6
        default: NS_ASSERT_MSG (false, "PPP Protocol number not defined!");
    }
    return 0;
}

void RdmaHw::RecoverQueue(Ptr<RdmaQueuePair> qp){
    qp->snd_nxt = qp->snd_una;
}

void RdmaHw::QpComplete(Ptr<RdmaQueuePair> qp){
    NS_ASSERT(!m_qpCompleteCallback.IsNull());
    if (m_cc_mode == 1){
        Simulator::Cancel(qp->mlx.m_eventUpdateAlpha);
        Simulator::Cancel(qp->mlx.m_eventDecreaseRate);
        Simulator::Cancel(qp->mlx.m_rpTimer);
    }

    // This callback will log info
    // It may also delete the rxQp on the receiver
    m_qpCompleteCallback(qp);

    qp->m_notifyAppFinish();

    // delete the qp
    DeleteQueuePair(qp);
}

void RdmaHw::SetLinkDown(Ptr<QbbNetDevice> dev){
    printf("RdmaHw: node:%u a link down\n", m_node->GetId());
}

void RdmaHw::AddTableEntry(Ipv4Address &dstAddr, uint32_t intf_idx){
    uint32_t dip = dstAddr.Get();
    m_rtTable[dip].push_back(intf_idx);
}

void RdmaHw::ClearTable(){
    m_rtTable.clear();
}

void RdmaHw::RedistributeQp(){
    // clear old qpGrp
    for (uint32_t i = 0; i < m_nic.size(); i++){
        if (m_nic[i].dev == NULL)
            continue;
        m_nic[i].qpGrp->Clear();
    }

    // redistribute qp
    for (auto &it : m_qpMap){
        Ptr<RdmaQueuePair> qp = it.second;
        uint32_t nic_idx = GetNicIdxOfQp(qp);
        m_nic[nic_idx].qpGrp->AddQp(qp);
        // Notify Nic
        m_nic[nic_idx].dev->ReassignedQp(qp);
    }
}

Ipv4Address RdmaHw::GetRandNextIp(uint16_t dst_id) {
    uint16_t next_ip_set_size = next_ip_table[dst_id].size();
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(0, (next_ip_set_size*PATH_AVG_WEIGHT)-1);
    uint16_t rand_num = distr(eng);
    uint16_t weight_sum = 0;

    for (uint16_t i = 0; i < next_ip_weight_table[dst_id].size(); i++) {
        uint16_t path_weight = next_ip_weight_table[dst_id][i];
        if ((weight_sum <= rand_num) && (rand_num < (weight_sum+path_weight))) {
            return next_ip_table[dst_id][i];
        } else {
            weight_sum += path_weight;
        }
    }
    std::cout << "ERROR: Get NULL NextIp!" << std::endl;
    return NULL;
}

void RdmaHw::GenPacket(Ptr<Packet> p, Ptr<RdmaQueuePair> qp, uint32_t seq) {
    #ifdef ENABLE_SEANET
    // add SeanetHeader
    SeanetHeader seanet;
    seanet.SetSEADstAddr (qp->dip.Get());
    #endif
    // add SeqTsHeader
    SeqTsHeader seqTs;
    seqTs.SetSeq (seq);
    seqTs.SetPG (qp->m_pg);
    // add udp header
    UdpHeader udpHeader;
    udpHeader.SetDestinationPort (qp->dport);
    udpHeader.SetSourcePort (qp->sport);
    // add ipv4 header
    Ipv4Header ipHeader;
    ipHeader.SetSource (qp->sip);
    #ifdef ENABLE_SEANET
    ipHeader.SetDestination (GetRandNextIp(qp->dst_id));
    seanet.SetSEANextAddr (GetRandNextIp(qp->dst_id).Get());
    p->AddHeader (seanet);
    #else
    ipHeader.SetDestination (qp->dip);
    #endif
    p->AddHeader (seqTs);
    p->AddHeader (udpHeader);
    ipHeader.SetProtocol (0x11);
    ipHeader.SetPayloadSize (p->GetSize());
    ipHeader.SetTtl (64);
    ipHeader.SetTos (0);
    ipHeader.SetIdentification (qp->m_ipid);
    p->AddHeader(ipHeader);
    // add ppp header
    PppHeader ppp;
    ppp.SetProtocol (0x0021); // EtherToPpp(0x800), see point-to-point-net-device.cc
    p->AddHeader (ppp);
}

Ptr<Packet> RdmaHw::GetRetransPacket(Ptr<RdmaQueuePair> qp) {
    auto& outstanding_pkts = qp->m_outstanding_pkts;
    uint64_t now_time = Simulator::Now().GetTimeStep();
    for (uint16_t i = 0; i < qp->m_outstanding_pkts.size(); i++) {
        if ((qp->m_outstanding_pkts[i].next_send_time <= now_time) && (qp->m_outstanding_pkts[i].acked == false)) {
            Ptr<Packet> p = Create<Packet> (qp->m_outstanding_pkts[i].size);
            GenPacket(p, qp, qp->m_outstanding_pkts[i].seq);
            qp->m_outstanding_pkts[i].last_send_time = now_time;
            qp->m_outstanding_pkts[i].next_send_time = now_time + uint64_t(m_maxRtt*2);
            #ifdef RDMA_HW_DEBUG_LOG
            std::cout << "RetransPacket11 : dst_ip " << (qp->dip.Get()>>24) << "." << ((qp->dip.Get()>>16)&0xff) << "." << ((qp->dip.Get()>>8)&0xff) << "." << ((qp->dip.Get())&0xff)\
                    << "; src_ip " << (qp->sip.Get()>>24) << "." << ((qp->sip.Get()>>16)&0xff) << "." << ((qp->sip.Get()>>8)&0xff) << "." << ((qp->sip.Get())&0xff)\
                    << "; seq " << qp->m_outstanding_pkts[i].seq << " now_time " << now_time << " last_send_time " << qp->m_outstanding_pkts[i].last_send_time << " next_send_time " << qp->m_outstanding_pkts[i].next_send_time << " m_maxRtt " << m_maxRtt << std::endl;
            #endif
            return p;
        }
    }
    return NULL;
}     

Ptr<Packet> RdmaHw::GetNxtPacket(Ptr<RdmaQueuePair> qp){
    Ptr<Packet> p = NULL;
    p = GetRetransPacket(qp);
    if (p != NULL)
        return p;

    // gen a new packet
    uint32_t payload_size = qp->GetBytesLeft();
    if (m_mtu < payload_size)
        payload_size = m_mtu;
    p = Create<Packet> (payload_size);
    GenPacket(p, qp, qp->snd_nxt);
    qp->m_outstanding_pkts.push_back(SeqMgr(qp->snd_nxt, payload_size, Simulator::Now().GetTimeStep(), Simulator::Now().GetTimeStep()+uint64_t(m_maxRtt*2), qp->sip));
    // update state
    qp->snd_nxt += payload_size;
    qp->m_ipid++;

    // return
    return p;
}

void RdmaHw::PktSent(Ptr<RdmaQueuePair> qp, Ptr<Packet> pkt, Time interframeGap){
    qp->lastPktSize = pkt->GetSize();
    UpdateNextAvail(qp, interframeGap, pkt->GetSize());
}

void RdmaHw::UpdateNextAvail(Ptr<RdmaQueuePair> qp, Time interframeGap, uint32_t pkt_size){
    Time sendingTime;
    if (m_rateBound)
        sendingTime = interframeGap + Seconds(qp->m_rate.CalculateTxTime(pkt_size));
    else
        sendingTime = interframeGap + Seconds(qp->m_max_rate.CalculateTxTime(pkt_size));
    qp->m_nextAvail = Simulator::Now() + sendingTime;
}

void RdmaHw::ChangeRate(Ptr<RdmaQueuePair> qp, DataRate new_rate){
    #if 1
    Time sendingTime = Seconds(qp->m_rate.CalculateTxTime(qp->lastPktSize));
    Time new_sendintTime = Seconds(new_rate.CalculateTxTime(qp->lastPktSize));
    qp->m_nextAvail = qp->m_nextAvail + new_sendintTime - sendingTime;
    // update nic's next avail event
    uint32_t nic_idx = GetNicIdxOfQp(qp);
    m_nic[nic_idx].dev->UpdateNextAvail(qp->m_nextAvail);
    #endif

    // change to new rate
    qp->m_rate = new_rate;
}

#define PRINT_LOG 0
/******************************
 * Mellanox's version of DCQCN
 *****************************/
void RdmaHw::UpdateAlphaMlx(Ptr<RdmaQueuePair> q){
    #if PRINT_LOG
    //std::cout << Simulator::Now() << " alpha update:" << m_node->GetId() << ' ' << q->mlx.m_alpha << ' ' << (int)q->mlx.m_alpha_cnp_arrived << '\n';
    //printf("%lu alpha update: %08x %08x %u %u %.6lf->", Simulator::Now().GetTimeStep(), q->sip.Get(), q->dip.Get(), q->sport, q->dport, q->mlx.m_alpha);
    #endif
    if (q->mlx.m_alpha_cnp_arrived){
        q->mlx.m_alpha = (1 - m_g)*q->mlx.m_alpha + m_g; 	//binary feedback
    }else {
        q->mlx.m_alpha = (1 - m_g)*q->mlx.m_alpha; 	//binary feedback
    }
    #if PRINT_LOG
    //printf("%.6lf\n", q->mlx.m_alpha);
    #endif
    q->mlx.m_alpha_cnp_arrived = false; // clear the CNP_arrived bit
    ScheduleUpdateAlphaMlx(q);
}
void RdmaHw::ScheduleUpdateAlphaMlx(Ptr<RdmaQueuePair> q){
    q->mlx.m_eventUpdateAlpha = Simulator::Schedule(MicroSeconds(m_alpha_resume_interval), &RdmaHw::UpdateAlphaMlx, this, q);
}

void RdmaHw::cnp_received_mlx(Ptr<RdmaQueuePair> q){
    q->mlx.m_alpha_cnp_arrived = true; // set CNP_arrived bit for alpha update
    q->mlx.m_decrease_cnp_arrived = true; // set CNP_arrived bit for rate decrease
    if (q->mlx.m_first_cnp){
        // init alpha
        q->mlx.m_alpha = 1;
        q->mlx.m_alpha_cnp_arrived = false;
        // schedule alpha update
        ScheduleUpdateAlphaMlx(q);
        // schedule rate decrease
        ScheduleDecreaseRateMlx(q, 1); // add 1 ns to make sure rate decrease is after alpha update
        // set rate on first CNP
        q->mlx.m_targetRate = q->m_rate = m_rateOnFirstCNP * q->m_rate;
        q->mlx.m_first_cnp = false;
    }
}

void RdmaHw::CheckRateDecreaseMlx(Ptr<RdmaQueuePair> q){
    ScheduleDecreaseRateMlx(q, 0);
    if (q->mlx.m_decrease_cnp_arrived){
        #if PRINT_LOG
        printf("%lu rate dec: %08x %08x %u %u (%0.3lf %.3lf)->", Simulator::Now().GetTimeStep(), q->sip.Get(), q->dip.Get(), q->sport, q->dport, q->mlx.m_targetRate.GetBitRate() * 1e-9, q->m_rate.GetBitRate() * 1e-9);
        #endif
        bool clamp = true;
        if (!m_EcnClampTgtRate){
            if (q->mlx.m_rpTimeStage == 0)
                clamp = false;
        }
        if (clamp)
            q->mlx.m_targetRate = q->m_rate;
        q->m_rate = std::max(m_minRate, q->m_rate * (1 - q->mlx.m_alpha / 2));
        // reset rate increase related things
        q->mlx.m_rpTimeStage = 0;
        q->mlx.m_decrease_cnp_arrived = false;
        Simulator::Cancel(q->mlx.m_rpTimer);
        q->mlx.m_rpTimer = Simulator::Schedule(MicroSeconds(m_rpgTimeReset), &RdmaHw::RateIncEventTimerMlx, this, q);
        #if PRINT_LOG
        printf("(%.3lf %.3lf)\n", q->mlx.m_targetRate.GetBitRate() * 1e-9, q->m_rate.GetBitRate() * 1e-9);
        #endif
    }
}
void RdmaHw::ScheduleDecreaseRateMlx(Ptr<RdmaQueuePair> q, uint32_t delta){
    q->mlx.m_eventDecreaseRate = Simulator::Schedule(MicroSeconds(m_rateDecreaseInterval) + NanoSeconds(delta), &RdmaHw::CheckRateDecreaseMlx, this, q);
}

void RdmaHw::RateIncEventTimerMlx(Ptr<RdmaQueuePair> q){
    q->mlx.m_rpTimer = Simulator::Schedule(MicroSeconds(m_rpgTimeReset), &RdmaHw::RateIncEventTimerMlx, this, q);
    RateIncEventMlx(q);
    q->mlx.m_rpTimeStage++;
}
void RdmaHw::RateIncEventMlx(Ptr<RdmaQueuePair> q){
    // check which increase phase: fast recovery, active increase, hyper increase
    if (q->mlx.m_rpTimeStage < m_rpgThreshold){ // fast recovery
        FastRecoveryMlx(q);
    }else if (q->mlx.m_rpTimeStage == m_rpgThreshold){ // active increase
        ActiveIncreaseMlx(q);
    }else { // hyper increase
        HyperIncreaseMlx(q);
    }
}

void RdmaHw::FastRecoveryMlx(Ptr<RdmaQueuePair> q){
    #if PRINT_LOG
    printf("%lu fast recovery: %08x %08x %u %u (%0.3lf %.3lf)->", Simulator::Now().GetTimeStep(), q->sip.Get(), q->dip.Get(), q->sport, q->dport, q->mlx.m_targetRate.GetBitRate() * 1e-9, q->m_rate.GetBitRate() * 1e-9);
    #endif
    q->m_rate = (q->m_rate / 2) + (q->mlx.m_targetRate / 2);
    #if PRINT_LOG
    printf("(%.3lf %.3lf)\n", q->mlx.m_targetRate.GetBitRate() * 1e-9, q->m_rate.GetBitRate() * 1e-9);
    #endif
}
void RdmaHw::ActiveIncreaseMlx(Ptr<RdmaQueuePair> q){
    #if PRINT_LOG
    printf("%lu active inc: %08x %08x %u %u (%0.3lf %.3lf)->", Simulator::Now().GetTimeStep(), q->sip.Get(), q->dip.Get(), q->sport, q->dport, q->mlx.m_targetRate.GetBitRate() * 1e-9, q->m_rate.GetBitRate() * 1e-9);
    #endif
    // get NIC
    uint32_t nic_idx = GetNicIdxOfQp(q);
    Ptr<QbbNetDevice> dev = m_nic[nic_idx].dev;
    // increate rate
    q->mlx.m_targetRate += m_rai;
    if (q->mlx.m_targetRate > dev->GetDataRate())
        q->mlx.m_targetRate = dev->GetDataRate();
    q->m_rate = (q->m_rate / 2) + (q->mlx.m_targetRate / 2);
    #if PRINT_LOG
    printf("(%.3lf %.3lf)\n", q->mlx.m_targetRate.GetBitRate() * 1e-9, q->m_rate.GetBitRate() * 1e-9);
    #endif
}
void RdmaHw::HyperIncreaseMlx(Ptr<RdmaQueuePair> q){
    #if PRINT_LOG
    printf("%lu hyper inc: %08x %08x %u %u (%0.3lf %.3lf)->", Simulator::Now().GetTimeStep(), q->sip.Get(), q->dip.Get(), q->sport, q->dport, q->mlx.m_targetRate.GetBitRate() * 1e-9, q->m_rate.GetBitRate() * 1e-9);
    #endif
    // get NIC
    uint32_t nic_idx = GetNicIdxOfQp(q);
    Ptr<QbbNetDevice> dev = m_nic[nic_idx].dev;
    // increate rate
    q->mlx.m_targetRate += m_rhai;
    if (q->mlx.m_targetRate > dev->GetDataRate())
        q->mlx.m_targetRate = dev->GetDataRate();
    q->m_rate = (q->m_rate / 2) + (q->mlx.m_targetRate / 2);
    #if PRINT_LOG
    printf("(%.3lf %.3lf)\n", q->mlx.m_targetRate.GetBitRate() * 1e-9, q->m_rate.GetBitRate() * 1e-9);
    #endif
}

/***********************
 * High Precision CC
 ***********************/
void RdmaHw::HandleAckHp(Ptr<RdmaQueuePair> qp, Ptr<Packet> p, CustomHeader &ch){
    uint32_t ack_seq = ch.ack.seq;
    // update rate
    if (ack_seq > qp->hp.m_lastUpdateSeq){ // if full RTT feedback is ready, do full update
        UpdateRateHp(qp, p, ch, false);
    }else{ // do fast react
        FastReactHp(qp, p, ch);
    }
}

void RdmaHw::UpdateRateHp(Ptr<RdmaQueuePair> qp, Ptr<Packet> p, CustomHeader &ch, bool fast_react){
    uint32_t next_seq = qp->snd_nxt;
    bool print = !fast_react || true;
    if (qp->hp.m_lastUpdateSeq == 0){ // first RTT
        qp->hp.m_lastUpdateSeq = next_seq;
        // store INT
        IntHeader &ih = ch.ack.ih;
        NS_ASSERT(ih.nhop <= IntHeader::maxHop);
        for (uint32_t i = 0; i < ih.nhop; i++)
            qp->hp.hop[i] = ih.hop[i];
        #if PRINT_LOG
        if (print){
            printf("%lu %s %08x %08x %u %u [%u,%u,%u]", Simulator::Now().GetTimeStep(), fast_react? "fast" : "update", qp->sip.Get(), qp->dip.Get(), qp->sport, qp->dport, qp->hp.m_lastUpdateSeq, ch.ack.seq, next_seq);
            for (uint32_t i = 0; i < ih.nhop; i++)
                printf(" %u %lu %lu", ih.hop[i].GetQlen(), ih.hop[i].GetBytes(), ih.hop[i].GetTime());
            printf("\n");
        }
        #endif
    }else {
        // check packet INT
        IntHeader &ih = ch.ack.ih;
        if (ih.nhop <= IntHeader::maxHop){
            double max_c = 0;
            bool inStable = false;
            #if PRINT_LOG
            if (print)
                printf("%lu %s %08x %08x %u %u [%u,%u,%u]", Simulator::Now().GetTimeStep(), fast_react? "fast" : "update", qp->sip.Get(), qp->dip.Get(), qp->sport, qp->dport, qp->hp.m_lastUpdateSeq, ch.ack.seq, next_seq);
            #endif
            // check each hop
            double U = 0;
            uint64_t dt = 0;
            bool updated[IntHeader::maxHop] = {false}, updated_any = false;
            NS_ASSERT(ih.nhop <= IntHeader::maxHop);
            for (uint32_t i = 0; i < ih.nhop; i++){
                if (m_sampleFeedback){
                    if (ih.hop[i].GetQlen() == 0 && fast_react)
                        continue;
                }
                updated[i] = updated_any = true;
                #if PRINT_LOG
                if (print)
                    printf(" %u(%u) %lu(%lu) %lu(%lu)", ih.hop[i].GetQlen(), qp->hp.hop[i].GetQlen(), ih.hop[i].GetBytes(), qp->hp.hop[i].GetBytes(), ih.hop[i].GetTime(), qp->hp.hop[i].GetTime());
                #endif
                uint64_t tau = ih.hop[i].GetTimeDelta(qp->hp.hop[i]);;
                double duration = tau * 1e-9;
                double txRate = (ih.hop[i].GetBytesDelta(qp->hp.hop[i])) * 8 / duration;
                double u = txRate / ih.hop[i].GetLineRate() + (double)std::min(ih.hop[i].GetQlen(), qp->hp.hop[i].GetQlen()) * qp->m_max_rate.GetBitRate() / ih.hop[i].GetLineRate() /qp->m_win;
                #if PRINT_LOG
                if (print)
                    printf(" %.3lf %.3lf", txRate, u);
                #endif
                if (!m_multipleRate){
                    // for aggregate (single R)
                    if (u > U){
                        U = u;
                        dt = tau;
                    }
                }else {
                    // for per hop (per hop R)
                    if (tau > qp->m_baseRtt)
                        tau = qp->m_baseRtt;
                    qp->hp.hopState[i].u = (qp->hp.hopState[i].u * (qp->m_baseRtt - tau) + u * tau) / double(qp->m_baseRtt);
                }
                qp->hp.hop[i] = ih.hop[i];
            }

            DataRate new_rate;
            int32_t new_incStage;
            DataRate new_rate_per_hop[IntHeader::maxHop];
            int32_t new_incStage_per_hop[IntHeader::maxHop];
            if (!m_multipleRate){
                // for aggregate (single R)
                if (updated_any){
                    if (dt > qp->m_baseRtt)
                        dt = qp->m_baseRtt;
                    qp->hp.u = (qp->hp.u * (qp->m_baseRtt - dt) + U * dt) / double(qp->m_baseRtt);
                    max_c = qp->hp.u / m_targetUtil;

                    if (max_c >= 1 || qp->hp.m_incStage >= m_miThresh){
                        new_rate = qp->hp.m_curRate / max_c + m_rai;
                        new_incStage = 0;
                    }else{
                        new_rate = qp->hp.m_curRate + m_rai;
                        new_incStage = qp->hp.m_incStage+1;
                    }
                    if (new_rate < m_minRate)
                        new_rate = m_minRate;
                    if (new_rate > qp->m_max_rate)
                        new_rate = qp->m_max_rate;
                    #if PRINT_LOG
                    if (print)
                        printf(" u=%.6lf U=%.3lf dt=%u max_c=%.3lf", qp->hp.u, U, dt, max_c);
                    #endif
                    #if PRINT_LOG
                    if (print)
                        printf(" rate:%.3lf->%.3lf\n", qp->hp.m_curRate.GetBitRate()*1e-9, new_rate.GetBitRate()*1e-9);
                    #endif
                }
            }else{
                // for per hop (per hop R)
                new_rate = qp->m_max_rate;
                for (uint32_t i = 0; i < ih.nhop; i++){
                    if (updated[i]){
                        double c = qp->hp.hopState[i].u / m_targetUtil;
                        if (c >= 1 || qp->hp.hopState[i].incStage >= m_miThresh){
                            new_rate_per_hop[i] = qp->hp.hopState[i].Rc / c + m_rai;
                            new_incStage_per_hop[i] = 0;
                        }else{
                            new_rate_per_hop[i] = qp->hp.hopState[i].Rc + m_rai;
                            new_incStage_per_hop[i] = qp->hp.hopState[i].incStage+1;
                        }
                        // bound rate
                        if (new_rate_per_hop[i] < m_minRate)
                            new_rate_per_hop[i] = m_minRate;
                        if (new_rate_per_hop[i] > qp->m_max_rate)
                            new_rate_per_hop[i] = qp->m_max_rate;
                        // find min new_rate
                        if (new_rate_per_hop[i] < new_rate)
                            new_rate = new_rate_per_hop[i];
                        #if PRINT_LOG
                        if (print)
                            printf(" [%u]u=%.6lf c=%.3lf", i, qp->hp.hopState[i].u, c);
                        #endif
                        #if PRINT_LOG
                        if (print)
                            printf(" %.3lf->%.3lf", qp->hp.hopState[i].Rc.GetBitRate()*1e-9, new_rate.GetBitRate()*1e-9);
                        #endif
                    }else{
                        if (qp->hp.hopState[i].Rc < new_rate)
                            new_rate = qp->hp.hopState[i].Rc;
                    }
                }
                #if PRINT_LOG
                printf("\n");
                #endif
            }
            if (updated_any)
                ChangeRate(qp, new_rate);
            if (!fast_react){
                if (updated_any){
                    qp->hp.m_curRate = new_rate;
                    qp->hp.m_incStage = new_incStage;
                }
                if (m_multipleRate){
                    // for per hop (per hop R)
                    for (uint32_t i = 0; i < ih.nhop; i++){
                        if (updated[i]){
                            qp->hp.hopState[i].Rc = new_rate_per_hop[i];
                            qp->hp.hopState[i].incStage = new_incStage_per_hop[i];
                        }
                    }
                }
            }
        }
        if (!fast_react){
            if (next_seq > qp->hp.m_lastUpdateSeq)
                qp->hp.m_lastUpdateSeq = next_seq; //+ rand() % 2 * m_mtu;
        }
    }
}

void RdmaHw::FastReactHp(Ptr<RdmaQueuePair> qp, Ptr<Packet> p, CustomHeader &ch){
    if (m_fast_react)
        UpdateRateHp(qp, p, ch, true);
}

/**********************
 * TIMELY
 *********************/
void RdmaHw::HandleAckTimely(Ptr<RdmaQueuePair> qp, Ptr<Packet> p, CustomHeader &ch){
    uint32_t ack_seq = ch.ack.seq;
    // update rate
    if (ack_seq > qp->tmly.m_lastUpdateSeq){ // if full RTT feedback is ready, do full update
        UpdateRateTimely(qp, p, ch, false);
    }else{ // do fast react
        FastReactTimely(qp, p, ch);
    }
}
void RdmaHw::UpdateRateTimely(Ptr<RdmaQueuePair> qp, Ptr<Packet> p, CustomHeader &ch, bool us){
    uint32_t next_seq = qp->snd_nxt;
    uint64_t rtt = Simulator::Now().GetTimeStep() - ch.ack.ih.ts;
    bool print = !us;
    if (qp->tmly.m_lastUpdateSeq != 0){ // not first RTT
        int64_t new_rtt_diff = (int64_t)rtt - (int64_t)qp->tmly.lastRtt;
        double rtt_diff = (1 - m_tmly_alpha) * qp->tmly.rttDiff + m_tmly_alpha * new_rtt_diff;
        double gradient = rtt_diff / m_tmly_minRtt;
        bool inc = false;
        double c = 0;
        #if PRINT_LOG
        if (print)
            printf("%lu node:%u rtt:%lu rttDiff:%.0lf gradient:%.3lf rate:%.3lf", Simulator::Now().GetTimeStep(), m_node->GetId(), rtt, rtt_diff, gradient, qp->tmly.m_curRate.GetBitRate() * 1e-9);
        #endif
        if (rtt < m_tmly_TLow){
            inc = true;
        }else if (rtt > m_tmly_THigh){
            c = 1 - m_tmly_beta * (1 - (double)m_tmly_THigh / rtt);
            inc = false;
        }else if (gradient <= 0){
            inc = true;
        }else{
            c = 1 - m_tmly_beta * gradient;
            if (c < 0)
                c = 0;
            inc = false;
        }
        if (inc){
            if (qp->tmly.m_incStage < 5){
                qp->m_rate = qp->tmly.m_curRate + m_rai;
            }else{
                qp->m_rate = qp->tmly.m_curRate + m_rhai;
            }
            if (qp->m_rate > qp->m_max_rate)
                qp->m_rate = qp->m_max_rate;
            if (!us){
                qp->tmly.m_curRate = qp->m_rate;
                qp->tmly.m_incStage++;
                qp->tmly.rttDiff = rtt_diff;
            }
        }else{
            qp->m_rate = std::max(m_minRate, qp->tmly.m_curRate * c); 
            if (!us){
                qp->tmly.m_curRate = qp->m_rate;
                qp->tmly.m_incStage = 0;
                qp->tmly.rttDiff = rtt_diff;
            }
        }
        #if PRINT_LOG
        if (print){
            printf(" %c %.3lf\n", inc? '^':'v', qp->m_rate.GetBitRate() * 1e-9);
        }
        #endif
    }
    if (!us && next_seq > qp->tmly.m_lastUpdateSeq){
        qp->tmly.m_lastUpdateSeq = next_seq;
        // update
        qp->tmly.lastRtt = rtt;
    }
}
void RdmaHw::FastReactTimely(Ptr<RdmaQueuePair> qp, Ptr<Packet> p, CustomHeader &ch){
}

/**********************
 * DCTCP
 *********************/
void RdmaHw::HandleAckDctcp(Ptr<RdmaQueuePair> qp, Ptr<Packet> p, CustomHeader &ch){
    uint32_t ack_seq = ch.ack.seq;
    uint8_t cnp = (ch.ack.flags >> qbbHeader::FLAG_CNP) & 1;
    bool new_batch = false;

    // update alpha
    qp->dctcp.m_ecnCnt += (cnp > 0);
    if (ack_seq > qp->dctcp.m_lastUpdateSeq){ // if full RTT feedback is ready, do alpha update
        #if PRINT_LOG
        printf("%lu %s %08x %08x %u %u [%u,%u,%u] %.3lf->", Simulator::Now().GetTimeStep(), "alpha", qp->sip.Get(), qp->dip.Get(), qp->sport, qp->dport, qp->dctcp.m_lastUpdateSeq, ch.ack.seq, qp->snd_nxt, qp->dctcp.m_alpha);
        #endif
        new_batch = true;
        if (qp->dctcp.m_lastUpdateSeq == 0){ // first RTT
            qp->dctcp.m_lastUpdateSeq = qp->snd_nxt;
            qp->dctcp.m_batchSizeOfAlpha = qp->snd_nxt / m_mtu + 1;
        }else {
            double frac = std::min(1.0, double(qp->dctcp.m_ecnCnt) / qp->dctcp.m_batchSizeOfAlpha);
            qp->dctcp.m_alpha = (1 - m_g) * qp->dctcp.m_alpha + m_g * frac;
            qp->dctcp.m_lastUpdateSeq = qp->snd_nxt;
            qp->dctcp.m_ecnCnt = 0;
            qp->dctcp.m_batchSizeOfAlpha = (qp->snd_nxt - ack_seq) / m_mtu + 1;
            #if PRINT_LOG
            printf("%.3lf F:%.3lf", qp->dctcp.m_alpha, frac);
            #endif
        }
        #if PRINT_LOG
        printf("\n");
        #endif
    }

    // check cwr exit
    if (qp->dctcp.m_caState == 1){
        if (ack_seq > qp->dctcp.m_highSeq)
            qp->dctcp.m_caState = 0;
    }

    // check if need to reduce rate: ECN and not in CWR
    if (cnp && qp->dctcp.m_caState == 0){
        #if PRINT_LOG
        printf("%lu %s %08x %08x %u %u %.3lf->", Simulator::Now().GetTimeStep(), "rate", qp->sip.Get(), qp->dip.Get(), qp->sport, qp->dport, qp->m_rate.GetBitRate()*1e-9);
        #endif
        qp->m_rate = std::max(m_minRate, qp->m_rate * (1 - qp->dctcp.m_alpha / 2));
        #if PRINT_LOG
        printf("%.3lf\n", qp->m_rate.GetBitRate() * 1e-9);
        #endif
        qp->dctcp.m_caState = 1;
        qp->dctcp.m_highSeq = qp->snd_nxt;
    }

    // additive inc
    if (qp->dctcp.m_caState == 0 && new_batch)
        qp->m_rate = std::min(qp->m_max_rate, qp->m_rate + m_dctcp_rai);
}

/*********************
 * HPCC-PINT
 ********************/
void RdmaHw::SetPintSmplThresh(double p){
    pint_smpl_thresh = (uint32_t)(65536 * p);
}
void RdmaHw::HandleAckHpPint(Ptr<RdmaQueuePair> qp, Ptr<Packet> p, CustomHeader &ch){
    uint32_t ack_seq = ch.ack.seq;
    if (rand() % 65536 >= pint_smpl_thresh)
            return;
    // update rate
    if (ack_seq > qp->hpccPint.m_lastUpdateSeq){ // if full RTT feedback is ready, do full update
            UpdateRateHpPint(qp, p, ch, false);
    }else{ // do fast react
            UpdateRateHpPint(qp, p, ch, true);
    }
}

void RdmaHw::UpdateRateHpPint(Ptr<RdmaQueuePair> qp, Ptr<Packet> p, CustomHeader &ch, bool fast_react){
    uint32_t next_seq = qp->snd_nxt;
    if (qp->hpccPint.m_lastUpdateSeq == 0){ // first RTT
            qp->hpccPint.m_lastUpdateSeq = next_seq;
    }else {
            // check packet INT
            IntHeader &ih = ch.ack.ih;
            double U = Pint::decode_u(ih.GetPower());

            DataRate new_rate;
            int32_t new_incStage;
            double max_c = U / m_targetUtil;

            if (max_c >= 1 || qp->hpccPint.m_incStage >= m_miThresh){
                    new_rate = qp->hpccPint.m_curRate / max_c + m_rai;
                    new_incStage = 0;
            }else{
                    new_rate = qp->hpccPint.m_curRate + m_rai;
                    new_incStage = qp->hpccPint.m_incStage+1;
            }
            if (new_rate < m_minRate)
                    new_rate = m_minRate;
            if (new_rate > qp->m_max_rate)
                    new_rate = qp->m_max_rate;
            ChangeRate(qp, new_rate);
            if (!fast_react){
                    qp->hpccPint.m_curRate = new_rate;
                    qp->hpccPint.m_incStage = new_incStage;
            }
            if (!fast_react){
                    if (next_seq > qp->hpccPint.m_lastUpdateSeq)
                            qp->hpccPint.m_lastUpdateSeq = next_seq; //+ rand() % 2 * m_mtu;
            }
    }
}

}
