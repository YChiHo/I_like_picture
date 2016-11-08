#include"http_parser.h"

using namespace std;
using namespace Tins;
string save_path = "wget -P /tmp/picture ";
struct packet{
    void snf_set(){
        SnifferConfiguration config;
        config.set_promisc_mode(true);
        config.set_filter("tcp");
        //config.set_timeout(1);
        Sniffer sniffer("dum0", config);
        sniffer.sniff_loop(make_sniffer_handler(this, &packet::handle));
    }
    bool handle(PDU& pdu){
        const EthernetII &eth = pdu.rfind_pdu<EthernetII>();
        const IP &ip = pdu.rfind_pdu<IP>();
        const TCP &tcp = pdu.rfind_pdu<TCP>();
        const RawPDU &rp = pdu.rfind_pdu<RawPDU>();
        const RawPDU::payload_type &pay_t = rp.payload();
        if(tcp.dport() == 80){
            if(html_parse((char*)pay_t.data(), eth, ip, tcp)){
                //html_parse((char*)pay_t.data(), eth, ip, tcp);
            }
        }
        return true;
    }
    void print(EthernetII eth, IP ip, TCP tcp, string total){
        cout<<"**************************************\n";
        cout<<"Packet Start\n";
        cout<<"Src : "<<hex<<eth.src_addr()<<"     "<<dec<<ip.src_addr()<<"\n";
        cout<<"Dst : "<<hex<<eth.dst_addr()<<"     "<<dec<<ip.dst_addr()<<"\n";
        cout<<"TCP size : "<<tcp.size()<<"\n";
        cout<<"\n\n\nTotal : "<<total<<"\n\n\n";
        cout<<"Packet End\n";
        cout<<"**************************************\n";
        system(total.c_str());
    }
    bool html_parse(char *payload, EthernetII eth, IP ip, TCP tcp){
        string total;
        string data = payload;
        Request_Line rl;
        Request_Line();
        rl.request_option();
        rl.request_line_parser(rl.Get_Line(data));
        rl.Request(data);
        total = save_path + rl.path + rl.domain;
        print(eth, ip, tcp, total);
        return true;
    }

};

int main(int argc, char **argv){
    packet pk;
    pk.snf_set();
}
