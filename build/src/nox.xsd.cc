#include "nox_main.hh"

namespace vigil {
namespace main {

const char* platform_configuration_schema =
  "<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
  "            xmlns:vigil=\"http://www.noxrepo.org/nox.xsd\"\n"
  "            targetNamespace=\"http://www.noxrepo.org/nox.xsd\">\n"
  "  <xsd:element name=\"nox\" type=\"vigil:NoxType\"/>\n"
  "\n"
  "  <xsd:complexType name=\"NoxType\">\n"
  "    <xsd:sequence>\n"
  "      <xsd:element name=\"events\" type=\"vigil:EventsType\"/>\n"
  "    </xsd:sequence>\n"
  "  </xsd:complexType>\n"
  "\n"
  "  <xsd:complexType name=\"EventsType\">\n"
  "    <xsd:sequence>\n"
  "      <xsd:element name=\"event\" type=\"vigil:EventType\"\n"
  "                   minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n"
  "    </xsd:sequence>\n"
  "  </xsd:complexType>\n"
  "\n"
  "  <xsd:complexType name=\"EventType\">\n"
  "    <xsd:sequence>\n"
  "      <xsd:element name=\"filter\" type=\"xsd:string\"\n"
  "                   minOccurs=\"1\" maxOccurs=\"unbounded\"/>\n"
  "    </xsd:sequence>\n"
  "    <xsd:attribute name=\"name\" type=\"xsd:string\"/>\n"
  "  </xsd:complexType>\n"
  "</xsd:schema>\n";

} // namespace main
} // namespace vigil
