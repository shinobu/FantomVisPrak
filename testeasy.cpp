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
          // ##### Variable #####
          size_t t= (size_t) 3;
          const std::pair< Cell::Type, size_t >  cellCounts =
          std::unique_ptr< ValueArray< size_t > >  	indices =

          // ##### Domain #####
            size_t extent[] = { (size_t)options.get< long >( "nx" ),
                                (size_t)options.get< long >( "ny" ),
                                (size_t)options.get< long >( "nz" ) };
            double origin[] = { -0.5 * options.get< double >( "dx" ) * ( options.get< long >( "nx" ) - 1 ),
                                -0.5 * options.get< double >( "dy" ) * ( options.get< long >( "ny" ) - 1 ),
                                -0.5 * options.get< double >( "dz" ) * ( options.get< long >( "nz" ) - 1 ) };
            double spacing[] = { options.get< double >( "dx" ), options.get< double >( "dy" ), options.get< double >( "dz" ) };

            std::shared_ptr< const DiscreteDomain< 3 > > mDomain = DomainFactory::makeDomainUniform( extent, origin, spacing );

            //##### Make Object #####
            //std::shared_ptr< const Grid< 3 > > mGrid = DomainFactory::makeGridStructured( *mDomain );
            //setResult( "grid", mGrid );

            //##### New Object #####
            static std::shared_ptr< const Grid< D > > fantom::DomainFactory::makeGridUnstructured(*mDomain, t, cellCounts, indices)


        }
    };
    AlgorithmRegister< CommitTutorialAlgorithm > dummy( "Tutorial/Commit2", "Generate a simple vector field." );
}
