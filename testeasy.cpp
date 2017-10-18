#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
using namespace fantom;
namespace
{
    class CommitTutorialAlgorithm : public DataAlgorithm
    {
    public:
        struct Options : public DataAlgorithm::Options
        {
            Options( fantom::Options::Control& control )
                : DataAlgorithm::Options( control )
            {
                add< long >( "nx", "", 10 );
                add< long >( "ny", "", 10 );
                add< long >( "nz", "", 10 );

                addSeparator();
                add< double >( "dx", "", 1.0 );
                add< double >( "dy", "", 1.0 );
                add< double >( "dz", "", 1.0 );
            }
        };

        //================================================================================================================

        struct DataOutputs : public DataAlgorithm::DataOutputs
        {
            DataOutputs( fantom::DataOutputs::Control& control )
                : DataAlgorithm::DataOutputs( control )
            {
                add< Grid< 3 > >( "grid" );
                add<Grid<3>>("house");
            }
        };

        //================================================================================================================

        CommitTutorialAlgorithm( InitData& data )
            : DataAlgorithm( data )
        {
        }

        //================================================================================================================

        virtual void execute( const Algorithm::Options& options, const volatile bool& /*abortFlag*/ ) override
        {
          //Wie viele Zelltypen
          size_t t = (size_t) 3;

          //Welcher Zelltyp
          //const voxel_pair p(fantom::Cell::Type::HEXAHEDRON,(size_t) 2);
          //const prism_pair p(fantom::Cell::Type::PRISM,(size_t) 1);
          //const pyramid_pair p(fantom::Cell::Type::PYRAMID,(size_t) 1);

          //Häufigkeit
          //const std::pair< Cell::Type, size_t > cellCounts[] = {voxel_pair, prism_pair, pyramid_pair};

          const std::pair< Cell::Type, size_t > cellCounts[] =
          {
            std::make_pair(Cell::HEXAHEDRON,(size_t) 2),
            std::make_pair(Cell::PRISM,(size_t) 1),
            std::make_pair(Cell::PYRAMID,(size_t) 1)
          };

          //Zellindices
          std::vector<size_t> vertice_vec = std::vector<size_t>();
          vertice_vec.resize(27);
          vertice_vec.push_back(0);
          vertice_vec.push_back(1);
          vertice_vec.push_back(2);
          vertice_vec.push_back(3);
          vertice_vec.push_back(4);
          vertice_vec.push_back(5);
          vertice_vec.push_back(6);
          vertice_vec.push_back(7);
          vertice_vec.push_back(1);
          vertice_vec.push_back(2);
          vertice_vec.push_back(5);
          vertice_vec.push_back(6);
          vertice_vec.push_back(8);
          vertice_vec.push_back(9);
          vertice_vec.push_back(10);
          vertice_vec.push_back(11);
          vertice_vec.push_back(4);
          vertice_vec.push_back(5);
          vertice_vec.push_back(6);
          vertice_vec.push_back(7);
          vertice_vec.push_back(12);
          vertice_vec.push_back(5);
          vertice_vec.push_back(6);
          vertice_vec.push_back(10);
          vertice_vec.push_back(11);
          vertice_vec.push_back(13);
          vertice_vec.push_back(14);

          // Raumpunkt
          std::vector<fantom::Vector3> raum_punkte = std::vector<fantom::Vector3>();
          raum_punkte.push_back({0,0,0});
          raum_punkte.push_back({1,0,0});
          raum_punkte.push_back({2,0,0});
          raum_punkte.push_back({0,1,0});
          raum_punkte.push_back({1,1,0});
          raum_punkte.push_back({2,1,0});
          raum_punkte.push_back({0,1,1});
          raum_punkte.push_back({1,1,1});
          raum_punkte.push_back({2,1,1});
          raum_punkte.push_back({0,0,1});
          raum_punkte.push_back({1,0,1});
          raum_punkte.push_back({2,0,1});
          raum_punkte.push_back({0,1,2});
          raum_punkte.push_back({1,1,2});
          raum_punkte.push_back({2,1,2});
          raum_punkte.push_back({0,0,2});
          raum_punkte.push_back({1,0,2});
          raum_punkte.push_back({2,0,2});

          // ##### Domain #####
            /*
            size_t extent[] = { (size_t)options.get< long >( "nx" ),
                                (size_t)options.get< long >( "ny" ),
                                (size_t)options.get< long >( "nz" ) };
            double origin[] = { -0.5 * options.get< double >( "dx" ) * ( options.get< long >( "nx" ) - 1 ),
                                -0.5 * options.get< double >( "dy" ) * ( options.get< long >( "ny" ) - 1 ),
                                -0.5 * options.get< double >( "dz" ) * ( options.get< long >( "nz" ) - 1 ) };
            double spacing[] = { options.get< double >( "dx" ), options.get< double >( "dy" ), options.get< double >( "dz" ) };

            std::shared_ptr< const DiscreteDomain< 3 > > mDomain = DomainFactory::makeDomainUniform( extent, origin, spacing );
            */
          std::shared_ptr< const DiscreteDomain< 3 > > mDomain = DomainFactory::makeDomainArbitrary( raum_punkte, Precision::FLOAT64 );

            //##### Make Object #####
            //std::shared_ptr< const Grid< 3 > > mGrid = DomainFactory::makeGridStructured( *mDomain );
            //setResult( "grid", mGrid );

            //##### New Object #####
            // PARAMS
            // 1. In welchem raum zeichne ich
            // 2. Wieviel Zelltypen benutze ich für mein Konstrukt
            // 3. Welcher Zelltyp wird wie oft verwendet
            // 4. Gibt die Ordnung der punkte an um bspw. einen Tetraeder zu beschreiben ~> suche

          std::shared_ptr< const Grid< 3 > > house = DomainFactory::makeGridUnstructured(*mDomain, t, cellCounts, fantom::makeValueArray<size_t>(vertice_vec, Precision::FLOAT64));
          setResult( "house", house );

        }
    };
    AlgorithmRegister< CommitTutorialAlgorithm > dummy( "Tutorial/Commit2", "Generate a simple vector field." );
}
