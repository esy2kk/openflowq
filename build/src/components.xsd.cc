#include "deployer.hh"

namespace vigil {

const char* Deployer::COMPONENTS_CONFIGURATION_SCHEMA =
  "<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
  "            xmlns:vigil=\"http://www.noxrepo.org/components.xsd\"\n"
  "            targetNamespace=\"http://www.noxrepo.org/components.xsd\">\n"
  "    <xsd:element name=\"components\" type=\"vigil:ComponentsType\"/>\n"
  "\n"
  "    <xsd:complexType name=\"ComponentsType\">\n"
  "        <xsd:sequence>\n"
  "            <xsd:element name=\"component\" type=\"vigil:ComponentType\"\n"
  "                         minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n"
  "        </xsd:sequence>\n"
  "    </xsd:complexType>\n"
  "\n"
  "    <xsd:complexType name=\"ComponentType\">\n"
  "        <xsd:sequence>\n"
  "            <xsd:element name=\"name\" type=\"xsd:string\"/>\n"
  "            <xsd:element name=\"library\" type=\"xsd:string\"\n"
  "                         minOccurs=\"0\"/>\n"
  "            <xsd:element name=\"dependency\" type=\"vigil:DependencyType\"\n"
  "                         minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n"
  "            <xsd:element name=\"python\" type=\"xsd:string\"\n"
  "                         minOccurs=\"0\"/>\n"
  "        </xsd:sequence>\n"
  "    </xsd:complexType>\n"
  "\n"
  "    <xsd:complexType name=\"DependencyType\">\n"
  "        <xsd:sequence>\n"
  "            <xsd:element name=\"name\" type=\"xsd:string\"/>\n"
  "        </xsd:sequence>\n"
  "    </xsd:complexType>\n"
  "</xsd:schema>\n";

} // namespace vigil
