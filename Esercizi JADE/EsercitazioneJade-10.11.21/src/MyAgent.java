import jade.core.AID;
import jade.core.Agent;
import jade.core.behaviours.CyclicBehaviour;
import jade.lang.acl.ACLMessage;
import jade.lang.acl.MessageTemplate;

import java.util.Objects;

public class MyAgent extends Agent {

    @Override
    protected void setup() {
        super.setup();
        System.out.println("Ho fatto partire un agente col nome " + getLocalName());
        this.addBehaviour(new CyclicBehaviour() {
            @Override
            public void action() {
                // Crea un filtro per il messaggio
                MessageTemplate msgTemplate = MessageTemplate.MatchPerformative(ACLMessage.QUERY_IF);
                //Creo una receive non bloccante
                ACLMessage msg = blockingReceive(msgTemplate);
                if (msg != null) {
                    // Messaggio ricevuto
                    AID sender = msg.getSender();
                    String title = msg.getContent();

                    if (Objects.equals(title, "alive")) {
                        ACLMessage responseMessage = new ACLMessage(ACLMessage.INFORM);
                        responseMessage.setContent("ready");
                        responseMessage.addReceiver(sender);
                        myAgent.send(responseMessage);
                        System.out.println("ho inviato il messaggio ready");
                    } else {
                        ACLMessage responseMessage = new ACLMessage(ACLMessage.NOT_UNDERSTOOD);
                        responseMessage.addReceiver(sender);
                        myAgent.send(responseMessage);
                        System.out.println("ho inviato il messaggio ready");
                    }

                }
            }
        });
    }
}
