#include "openflowq.hh"
#include "assert.hh"
#include "netinet++/ethernet.hh"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace vigil;
//using namespace vigil::container;
//using namespace vigil::applications;

namespace vigil {
//namespace applications {
//{
  static Vlog_module lg("openflowq");
  
  void openflowq::configure(const Configuration* c) 
  {
    lg.dbg(" Configure called ");

    resolve(ht);
    resolve(ri);
    resolve(frr);
    resolve(frr_qos);

    post_flow_record = SIMPLEROUTING_POST_RECORD_DEFAULT;

    register_handler<Packet_in_event>
      (boost::bind(&openflowq::handle_pkt_in, this, _1));

    //Get commandline arguments
    const hash_map<string, string> argmap = \
      c->get_arguments_list();
    hash_map<string, string>::const_iterator i = \
      argmap.find("postrecord");
    if (i != argmap.end())
    {
      if (i->second == "true")
        post_flow_record = true;
      else if (i->second == "false")
        post_flow_record = false;
      else
        VLOG_WARN(lg, "Cannot parse argument postrecord=%s",
                  i->second.c_str());
    }
  }

  Disposition openflowq::handle_pkt_in(const Event& e)
  {
    const Packet_in_event& pie = assert_cast<const Packet_in_event&>(e);

    //Best effort Service
    if (pie.flow.nw_tos == 0)
    {
      //Skip LLDP
      if (pie.flow.dl_type == ethernet::LLDP)
        return CONTINUE;

      const hosttracker::location dloc = ht->get_latest_location(pie.flow.dl_dst);
      bool routed = false;

      //Route or at least try
      if (!dloc.dpid.empty())
      {
        network::route rte(pie.datapath_id, pie.in_port);
        network::termination endpt(dloc.dpid, dloc.port);
        if (ri->get_shortest_path(endpt, rte))
        {
          ri->install_route(pie.flow, rte, pie.buffer_id);
          if (post_flow_record)
            frr->set(pie.flow, rte);
          routed = true;
        }
      }

      //Failed to route, flood
      if (!routed)
      {
        //Flood
        VLOG_DBG(lg, "Sending packet of %s via control",
                 pie.flow.to_string().c_str());
        if (pie.buffer_id == ((uint32_t) -1))
          send_openflow_packet(pie.datapath_id, *(pie.buf),
                               OFPP_FLOOD, pie.in_port, false);
        else
          send_openflow_packet(pie.datapath_id, pie.buffer_id,
                               OFPP_FLOOD, pie.in_port, false);
      }

      return CONTINUE;
    }

    //QoS Sevice

    //Skip LLDP
    if (pie.flow.dl_type == ethernet::LLDP)
      return CONTINUE;

    const hosttracker::location dloc = ht->get_latest_location(pie.flow.dl_dst);
    bool routed = false;

    //frr_qos check
    //Route or at least try
    if (!dloc.dpid.empty())
    {
      network::route rte_qos(pie.datapath_id, pie.in_port);
      network::termination endpt(dloc.dpid, dloc.port);
      //Service request through ToS field
      if (ri->get_shortest_path(endpt, rte_qos))
      {
        ri->install_route(pie.flow, rte_qos, pie.buffer_id);
        if (post_flow_record)
          frr_qos->set(pie.flow, rte_qos);
        routed = true;
      }
    }

    //Failed to route, flood
    if (!routed)
    {
      //Flood
      VLOG_DBG(lg, "Sending packet of %s via control",
               pie.flow.to_string().c_str());
      if (pie.buffer_id == ((uint32_t) -1))
        send_openflow_packet(pie.datapath_id, *(pie.buf),
                             OFPP_FLOOD, pie.in_port, false);
      else
        send_openflow_packet(pie.datapath_id, pie.buffer_id,
                             OFPP_FLOOD, pie.in_port, false);
    }

    //Post timer
    timeval tv = { 1, 0 };
    post(boost::bind(&openflowq::timer_callback, this), tv);

    return CONTINUE;
  }

  void openflowq::install()
  {
    lg.dbg(" Install called ");
  }

  void openflowq::timer_callback() {
    //using namespace std;
    cout << "One second has passed " << endl;
    timeval tv = { 1, 0 };
    post(boost::bind(&openflowq::timer_callback,this), tv);
  }

  void openflowq::getInstance(const Context* c,
				  openflowq*& component)
  {
    component = dynamic_cast<openflowq*>
      (c->get_by_interface(container::Interface_description
			      (typeid(openflowq).name())));
  }

  REGISTER_COMPONENT(Simple_component_factory<openflowq>,
		     openflowq);
} // vigil namespace
//}
