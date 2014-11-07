import java.util.LinkedList;
import java.util.List;

public class UnknownTypeList
{
    public enum Types
    {
        TYPEBASE("TypeBase"),
        TYPEA("TypeA"),
        TYPEB("TypeB");
        
        private final String name;
        
        private Types(String name)
        {
            this.name = name;
        }
        
        public List<? extends TypeBase> getList()
        {
            List<? extends TypeBase> l;
            switch(this)
            {
                case TYPEA:
                    l = new LinkedList<TypeA>();
                    break;
                case TYPEB:
                    l = new LinkedList<TypeA>();
                    break;
                default:
                    l = new LinkedList<TypeBase>();
            }
            return l;
        }
        
        public static List<? extends TypeBase> getList(String name)
        {
            List<? extends TypeBase> l = TYPEBASE.getList();
            if (!name.isEmpty())
            {
                for (Types t : values())
                {
                    if (t.name.equals(name))
                    {
                        l = t.getList();
                    }
                }
            }
            return l;
        }
    };
    
    public static void main(String[] args)
    {
        UnknownTypeList utl = new UnknownTypeList();
        List<? extends TypeBase> l;
        l = Types.getList("TypeA");
        for (TypeBase t : l)
        {
            System.out.println("Object: " + t.getName());
        }
    }
}