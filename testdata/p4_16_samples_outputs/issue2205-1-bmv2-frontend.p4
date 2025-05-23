#include <core.p4>
#define V1MODEL_VERSION 20180101
#include <v1model.p4>

header ethernet_t {
    bit<48> dst_addr;
    bit<48> src_addr;
    bit<16> eth_type;
}

struct Headers {
    ethernet_t eth_hdr;
}

struct Meta {
}

parser p(packet_in pkt, out Headers hdr, inout Meta m, inout standard_metadata_t sm) {
    state start {
        pkt.extract<ethernet_t>(hdr.eth_hdr);
        transition accept;
    }
}

control ingress(inout Headers h, inout Meta m, inout standard_metadata_t sm) {
    @name("ingress.tmp") bit<16> tmp;
    @name("ingress.tmp_0") bit<16> tmp_0;
    @name("ingress.tmp_1") bool tmp_1;
    @name("ingress.val_0") bit<16> val;
    @name("ingress.retval") bit<16> retval;
    @name("ingress.inlinedRetval") bit<16> inlinedRetval_0;
    apply {
        tmp = h.eth_hdr.eth_type;
        val = 16w182;
        retval = 16w2;
        h.eth_hdr.eth_type = val;
        inlinedRetval_0 = retval;
        tmp_0 = inlinedRetval_0;
        tmp_1 = tmp == tmp_0;
        if (tmp_1) {
            h.eth_hdr.src_addr = 48w1;
        }
    }
}

control vrfy(inout Headers h, inout Meta m) {
    apply {
    }
}

control update(inout Headers h, inout Meta m) {
    apply {
    }
}

control egress(inout Headers h, inout Meta m, inout standard_metadata_t sm) {
    apply {
    }
}

control deparser(packet_out pkt, in Headers h) {
    apply {
        pkt.emit<Headers>(h);
    }
}

V1Switch<Headers, Meta>(p(), vrfy(), ingress(), egress(), update(), deparser()) main;
